#include "include/manager.h"
#include <errno.h>

typedef struct slave_info {
    int app_to_slave[2]; // File descriptors connecting app to slave
    int slave_to_app[2]; // File descriptors connecting slave to app
    pid_t pid;           // Slave's pid 
    int files_solved;    // Amount of files solved by the slave
} slave_info;

void validate_args(int argc, int q_files);

char * validate_token(char * buf);

int main (int argc, char * argv[]) {
    int q_files = 0;
    char * files[argc];

    // i initial value = 1 because first argument is path
    for (int i = 1; i < argc; i++) {
        if (is_file(argv[i])) {
            files[q_files++] = argv[i];
        }
    }

    validate_args(argc, q_files);

    int number_slaves = MIN(SLAVES_FROM_FILES(q_files), MAX_SLAVES);
    slave_info slaves[number_slaves];

    FILE * output = create_file("respuesta.txt", "w");
    
    fd_set fd_read_set[2];

    // Initializes the set on NULL
    FD_ZERO(&fd_read_set[ORIGINAL]);

    for (int i = 0; i < number_slaves; i++) {
        create_pipe(slaves[i].app_to_slave);
        create_pipe(slaves[i].slave_to_app);

        // Includes fd in fd_set (we add all read fd to the set)
        FD_SET(slaves[i].slave_to_app[READ], &(fd_read_set[ORIGINAL]));
    }

    // Backup original set
    fd_read_set[BACKUP] = fd_read_set[ORIGINAL];

    // In case a shared memory or semaphore already exists with said name we unlink them
    shm_unlink(SHM_NAME);
    sem_unlink(SEM_NAME);
    
    // Creating shared memory and semaphore 
    shm_info shm;
    sem_info reading_sem;

    shm.name = SHM_NAME;
    reading_sem.name = SEM_NAME;

    create_shm(&shm);
    create_sem(&reading_sem);

    // Turning off print buffering
    setvbuf(stdout, NULL, _IONBF, 0);

    /* --- BROADCAST FOR VISTA PROCESS --- */
    printf("%s\n", shm.name);
    printf("%s\n", reading_sem.name);

    // Waiting for vista process to appear
    sleep(5);

    // Creating slaves
    pid_t last_pid = 1;
    int current_slave;
    for (current_slave = 0; current_slave < number_slaves && last_pid != 0; current_slave++) {
        last_pid = create_slave();
        slaves[current_slave].pid = last_pid;
    }

    if (last_pid == 0) {
        // Child process
        // Closing unused pipes
        for (int i = 0; i < number_slaves; i++) {
            if (i != current_slave - 1) {
                close_fd(slaves[i].app_to_slave[READ]);
                close_fd(slaves[i].app_to_slave[WRITE]);
                close_fd(slaves[i].slave_to_app[READ]);
                close_fd(slaves[i].slave_to_app[WRITE]);
            }
        }

        slave(slaves[current_slave - 1].app_to_slave, slaves[current_slave - 1].slave_to_app);  

    } else {
        // Parent process
        char ans[MAX_LEN] = {0};
        md5_info result;
        int current_file = 0, files_read = 0;
        
        // Initializing result on null
        memset(&result, 0, sizeof(md5_info));
        
        // Closing unused pipes
        for (int i = 0; i < number_slaves; i++) {
            close_fd(slaves[i].app_to_slave[READ]);
            close_fd(slaves[i].slave_to_app[WRITE]);
        }

        // Distribution of INITIAL_FILES_PER_SLAVE 
        // Initial distribution = number_slaves * INITIAL_FILES_PER_SLAVE 
        for (int current_slave = 0; current_file < number_slaves * INITIAL_FILES_PER_SLAVE && current_file < q_files; current_slave++) {
            for (int i = 0; i < INITIAL_FILES_PER_SLAVE && current_file < q_files; i++) {
                // Writing the file's name to the pipe
                write_fd(slaves[current_slave].app_to_slave[WRITE], &(files[current_file]), sizeof(char *));
                current_file++;
            }
            slaves[current_slave].files_solved = 0;
        }
        
        // Reading results
        while (files_read < q_files) {
            // Wait for a slave to finish with select_fd
            select_fd(FD_SETSIZE, &(fd_read_set[ORIGINAL]), NULL, NULL, NULL);
            for (int i = 0; i < number_slaves; i++) {
                if (FD_ISSET(slaves[i].slave_to_app[READ], &(fd_read_set[ORIGINAL]))) {
                
                    read_fd(slaves[i].slave_to_app[READ], ans, MAX_LEN * sizeof(char));

                    // Updating files_solved
                    slaves[i].files_solved++;

                    // Filling md5_info fields
                    strcpy(result.hash, validate_token(ans));
                    strcpy(result.file_name, validate_token(NULL));
                    result.pid = slaves[i].pid;
                    result.files_left = q_files - files_read;

                    // Writing result to the shared memory
                    write_shm(shm.fd, &result, sizeof(md5_info), files_read);
                    post_sem(&reading_sem);

                    // Writing result to output file
                    fprintf(output, "MD5: %s -- NAME: %s -- PID: %d\n", result.hash, result.file_name, result.pid);

                    // Distribution of remaining files
                    // Only give files to a slave once it has finished its initial distribution
                    if (current_file < q_files && slaves[i].files_solved >= INITIAL_FILES_PER_SLAVE) {
                        write_fd(slaves[i].app_to_slave[WRITE], &(files[current_file]), sizeof(char *));
                        current_file++;
                    }
                    
                    // Updating files_read
                    files_read++;
                }
            }

            // Going back to the original state
            fd_read_set[ORIGINAL] = fd_read_set[BACKUP];
        }

        // Closing remaining pipes and killing all slave processes
        for (int i = 0; i < number_slaves; i++) {
            close_fd(slaves[i].app_to_slave[WRITE]);
            close_fd(slaves[i].slave_to_app[READ]);
            kill_slave(slaves[i].pid);
        }
    }

    // Closing shared memory, semaphore and output file
    close_shm(&shm);

    // No need to wait for vista process, when closing the pipes
    // the processes will receive a read error and release their resources 
    close_sem(&reading_sem);
    close_file(output);

    unlink_shm(&shm);
    unlink_sem(&reading_sem);
    
    return 0;
}

void validate_args(int argc, int q_files) {
    if (argc <= 1 || q_files == 0){
        errno = ENOENT;
        perror("No files found.");
        exit(ERR_NO_FILES_FOUND); 
    }
}

char * validate_token(char * buf) {
    char * token;
    if ((token = strtok(buf, " ")) == NULL) {
        perror("Invalid answer from slave");
        exit(ERR_INVALID_ANSWER);
    }
    return token;
}
 