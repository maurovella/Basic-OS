#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define READ 0
#define WRITE 1
#define FILES_PER_SLAVE 2


typedef struct slave_info {
    int app_to_slave[2]; // File descriptors connecting app to slave
    int slave_to_app[2]; // File descriptors connecting slave to app
    int pid; // Slave's pid (pid_t or int?)
} slave_info;

int main (int argc, char * argv[]) {
    int cant_files = 0;
    char * files[argc];

    // i initial value = 1 because first argument = path
    for (int i = 1; i < argc; i++) {
        // TO-DO: is_file function
        // is_file returns 1 if parameter is a regular file
        if (is_file(argv[i])) {
            files[cant_files++] = argv[i];
        }
    }

    if (argc <= 1 || cant_files == 0){
        perror("No files found");
        exit(1); // TO-DO: error codes
    }

    // +1 because you need at least 1 slave
    int number_slaves = cant_files / FILES_PER_SLAVE + 1;

    slave_info slaves[number_slaves];

    // Creating output file
    FILE * output = fopen("respuesta.txt", "w");
    if (output == NULL) {
        perror("Error creating output file");
        exit(1); // Error codes
    }

    // Creating pipes between master and slave
    
    fd_set fd_read_set;

    // Initializes the set on NULL
    FD_ZERO(&fd_read_set);

    for (int i = 0; i < number_slaves; i++) {
        // TO-DO: create_pipe function
        if (pipe(slaves[i].app_to_slave) == -1) {
            perror("Error creating pipe");
            exit(1); // Error codes
        }
        if (pipe(slaves[i].slave_to_app) == -1) {
            perror("Error creating pipe");
            exit(1); // Error codes
        }

        // Includes fd in fd_set (we add all read fd to the set)
        FD_SET(slaves[i].slave_to_app[READ], &fd_read_set);
    }

    // Creating shared memory and semaphores 
    // TO-DO: shared memory and semaphore functions
    // ...

    // Creating slaves
    // ...
    return 0;
}
