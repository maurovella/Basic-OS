#include "include/manager.h"

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define READ 0
#define WRITE 1
#define FILES_PER_SLAVE 5
#define SLAVES_FROM_FILES(cant_files) (cant_files / FILES_PER_SLAVE + 1)

typedef struct slave_info {
    uint32_t app_to_slave[2]; // File descriptors connecting app to slave
    uint32_t slave_to_app[2]; // File descriptors connecting slave to app
    uint32_t pid; // Slave's pid (pid_t or int?)
} slave_info;

uint32_t main (uint32_t argc, uint8_t * argv[]) {
    uint32_t cant_files = 0;
    uint8_t * files[argc];

    // i initial value = 1 because first argument is path
    for (uint32_t i = 1; i < argc; i++) {
        // is_file returns 1 if parameter is a regular file
        if (is_file(argv[i])) {
            files[cant_files++] = argv[i];
        }
    }

    if (argc <= 1 || cant_files == 0){
        perror("No files found");
        exit(NO_FILES_FOUND); 
    }

    uint32_t number_slaves = SLAVES_FROM_FILES(cant_files);
    slave_info slaves[number_slaves];

    FILE * output = create_file("respuesta.txt", "w");
    
    // Creating pipes between master and slave
    
    fd_set fd_read_set, fd_backup_read_set;

    // Initializes the set on NULL
    FD_ZERO(&fd_read_set);

    for (uint32_t i = 0; i < number_slaves; i++) {
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

