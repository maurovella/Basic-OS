#include "include/manager.h"

#define READ 0
#define WRITE 1
#define FILES_PER_SLAVE 5
#define SLAVES_FROM_FILES(cant_files) (cant_files / FILES_PER_SLAVE + 1)

typedef struct slave_info {
    int app_to_slave[2]; // File descriptors connecting app to slave
    int slave_to_app[2]; // File descriptors connecting slave to app
    pid_t pid; // Slave's pid (pid_t or int?)
}slave_info;

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
    

    int number_slaves = SLAVES_FROM_FILES(cant_files);
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
    // TO-DO: shared memory and semaphore functions
    // ...

    // Creating slaves
    // ...
    return 0;
}

void validate_files(int argc, int cant_files) {
    if (argc <= 1 || cant_files == 0){
        perror("No files found.");
        exit(ERR_NO_FILES_FOUND); 
    }
}