// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/manager.h"
#include <errno.h>
#define READ 0
#define WRITE 1
#define INITIAL_FILES_PER_SLAVE 3
#define SLAVES_FROM_FILES(cant_files) (((cant_files) / (INITIAL_FILES_PER_SLAVE * 3)) + 1)
#define MAX_SLAVES 50
#define MAX_LEN 256
#define MIN(a, b) ((a) <= (b) ? (a) : (b))

typedef struct slave_info {
    int app_to_slave[2]; // File descriptors connecting app to slave
    int slave_to_app[2]; // File descriptors connecting slave to app
    pid_t pid; // Slave's pid (pid_t or int?)
    char * file_name; 
} slave_info;

void validate_files(int argc, int cant_files);

int main (int argc, char * argv[]) {
    int cant_files = 0;
    char * files[argc];

    // i initial value = 1 because first argument is path
    for (int i = 1; i < argc; i++) {
        // is_file returns 1 if parameter is a regular file
        if (is_file(argv[i])) {
            files[cant_files++] = argv[i];
        }
    }

    validate_files(argc, cant_files);
    printf("%d\n", cant_files);

    int number_slaves = MIN(SLAVES_FROM_FILES(cant_files), MAX_SLAVES);
    slave_info slaves[number_slaves];

    FILE * output = create_file("respuesta.txt", "w");
    
    // Creating pipes between master and slave
    
    fd_set fd_read_set, fd_backup_read_set;

    // Initializes the set on NULL
    FD_ZERO(&fd_read_set);

    for (int i = 0; i < number_slaves; i++) {
        create_pipe(slaves[i].app_to_slave);
        create_pipe(slaves[i].slave_to_app);

        // Includes fd in fd_set (we add all read fd to the set)
        FD_SET(slaves[i].slave_to_app[READ], &fd_read_set);
    }

    fd_backup_read_set = fd_read_set;
    
    // Creating shared memory and semaphores 
    shm_info shm;
    sem_info reading_sem, closing_sem;

    shm.name = "/shm";
    reading_sem.name = "/sem";
    closing_sem.name = "/closing_name";

    create_shm(&shm);
    create_sem(&reading_sem);
    create_sem(&closing_sem);

    printf("%s\n", shm.name);
    printf("%s\n", reading_sem.name);
    printf("%s\n", closing_sem.name);

    post_sem(&closing_sem);
    
    
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
        // Call slave
        slave(slaves[current_slave - 1].app_to_slave, slaves[current_slave - 1].slave_to_app);
    } else {
        // Parent process
        char ans[MD5_SIZE + 1] = {0};
        md5_info result;
        // Closing unused pipes
        for (int i = 0; i < number_slaves; i++) {
            close_fd(slaves[i].app_to_slave[READ]);
            close_fd(slaves[i].slave_to_app[WRITE]);
        }
        
        //Distribution of initial_files_per_slave files per slave
        int  current_file = 0, files_read = 0;
        
        // n_slaves * init_files = init_dist
        for (int current_slave = 0; current_file < number_slaves * INITIAL_FILES_PER_SLAVE; current_slave++) {
            for (int i = 0; i < INITIAL_FILES_PER_SLAVE && current_file < cant_files; i++) {
                write_fd(slaves[current_slave].app_to_slave[WRITE], &(files[current_file]), sizeof(char *));
                slaves[current_slave].file_name = files[current_file++];
            }
        }
        // Reading results
        while (files_read < cant_files) {
            // Wait for a slave to finish with select_fd
            select_fd(FD_SETSIZE, &fd_read_set, NULL, NULL, NULL);
            for (int i = 0; i < number_slaves; i++) {
                if (FD_ISSET(slaves[i].slave_to_app[READ], &fd_read_set)) {
                    // Read result with void read_fd
                    read_fd(slaves[i].slave_to_app[READ], ans, MD5_SIZE * sizeof(char));
                    strcpy(result.hash, ans);
                    result.pid = slaves[i].pid;
                    strcpy(result.file_name, slaves[i].file_name);
                    // Write result to output file
                    fprintf(output, "MD5: %s -- NAME: %s -- PID: %d\n", result.hash, result.file_name, result.pid);    
                    // Add new file to slave
                    if (current_file < cant_files) {
                        write_fd(slaves[i].app_to_slave[WRITE], &(files[current_file]), sizeof(char *));
                        slaves[i].file_name = files[current_file];
                        current_file++;
                    }
                    // Update files_read
                    files_read++;
                }
            }
            fd_read_set = fd_backup_read_set;
        }
        // Closing remaining pipes and killing all slave processes
        for (int i = 0; i < number_slaves; i++) {

            close_fd(slaves[i].app_to_slave[WRITE]);
            close_fd(slaves[i].slave_to_app[READ]);
            kill_slave(slaves[i].pid);
            
        }
    }

    close_shm(&shm);
    close_sem(&reading_sem);

    wait_sem(&closing_sem);
    close_sem(&closing_sem);
    
    close_file(output);

    unlink_shm(&shm);
    unlink_sem(&reading_sem);
    unlink_sem(&closing_sem);
    
    return 0;
}

void validate_files(int argc, int cant_files) {
    if (argc <= 1 || cant_files == 0){
        errno = ENOENT;
        perror("No files found.");
        exit(ERR_NO_FILES_FOUND); 
    }
}