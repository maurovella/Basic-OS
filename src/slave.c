#include "./include/manager.h"

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define READ 0
#define WRITE 1
#define ORIGINAL 0
#define BACKUP 1
#define MD5_SIZE 32

// TO-DO: close_fd function
// TO-DO: redirect_fd function
typedef struct subslave_info {
    uint32_t pipe_fd[2];
    fd_set sets_fd[2];  // [0] -> original, [1] -> backup
} subslave_info;

uint32_t slave (uint32_t app_to_slave[2], uint32_t slave_to_app[2]) {

    uint8_t * args[] = {"md5sum", NULL, NULL}; // Second arg = file name -> read from app
    uint8_t output[MD5_SIZE + 1] = {0};

    // app_to_slave fd: [0] -> read, [1] -> write
    // slave_to_app fd: [0] -> read, [1] -> write

    // slave -> escribe en app (app_to_slave[WRITE])
    // app -> lee en slave (app_to_slave[READ])

    close_fd(app_to_slave[WRITE]);
    close_fd(app_to_slave[READ]);
    
    // app_to_slave fd: [0] -> read, [1] -> null
    // slave_to_app fd: [0] -> null, [1] -> write
    // write(1,buff,size) write(slave_to_app[WRITE])

    fd_set app_to_slave_set[2];
    FDZERO(&app_to_slave[ORIGINAL]);
    FDSET(app_to_slave[READ], &app_to_slave_set[ORIGINAL]);
    app_to_slave_set[BACKUP] = app_to_slave_set[ORIGINAL];

    subslave_info subslave;

    create_pipe(subslave.pipe_fd);

    close_fd(0);
    dup_fd(subslave.pipe_fd[READ], 0);

    close_fd(1);
    dup_fd(subslave.pipe_fd[WRITE], 1);

    FDZERO(&subslave.sets_fd[ORIGINAL]);
    FDSET(subslave.pipe_fd[READ], &subslave.sets_fd[ORIGINAL]);
    subslave.sets_fd[BACKUP] = subslave.sets_fd[ORIGINAL];
}