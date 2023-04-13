// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "./include/manager.h"

#define READ 0
#define WRITE 1
#define ORIGINAL 0
#define BACKUP 1
#define MD5_SIZE 32


typedef struct subslave_info {
    int pipe_fd[2];
    fd_set sets_fd[2];  // [0] -> original, [1] -> backup
} subslave_info;

int finished = 0;

int slave (int app_to_slave[2], int slave_to_app[2]) {

    char * args[] = {"md5sum", NULL, NULL}; // Second arg = file name -> read from app
    char output[MD5_SIZE + 1] = {0};
    char * file_name;

    // app_to_slave fd: [0] -> read, [1] -> write
    // slave_to_app fd: [0] -> read, [1] -> write

    // slave -> escribe en app (slave_to_app[WRITE])
    // app -> lee en slave (app_to_slave[READ])

    close_fd(app_to_slave[WRITE]);
    close_fd(slave_to_app[READ]);
    
    // app_to_slave fd: [0] -> read, [1] -> null
    // slave_to_app fd: [0] -> null, [1] -> write

    fd_set app_to_slave_set[2];
    FDZERO(&app_to_slave[ORIGINAL]);
    FDSET(app_to_slave[READ], &app_to_slave_set[ORIGINAL]);
    app_to_slave_set[BACKUP] = app_to_slave_set[ORIGINAL];

    subslave_info subslave;

    create_pipe(subslave.pipe_fd);

    subslave.pipe_fd[READ] = dup_fd(subslave.pipe_fd[READ], 0);

    subslave.pipe_fd[WRITE] = dup_fd(subslave.pipe_fd[WRITE], 1);

    FDZERO(&subslave.sets_fd[ORIGINAL]);
    FDSET(subslave.pipe_fd[READ], &subslave.sets_fd[ORIGINAL]);
    subslave.sets_fd[BACKUP] = subslave.sets_fd[ORIGINAL];

    while (!finished) {
        select_fd(FD_SETSIZE, &(app_to_slave_set[ORIGINAL]), NULL, NULL, NULL);
        app_to_slave_set[ORIGINAL] = app_to_slave_set[BACKUP];
        read_fd(app_to_slave[READ], &file_name, sizeof(char *)); 
        if (create_slave() == 0) {
            args[1] = file_name;
            execvp("md5sum", args);
        } else {
            md5_info result;
            read_fd(subslave.pipe_fd[READ], result.hash, sizeof(result.hash));
            result.pid = getpid();
        }
        
    }
    
    return 0;    
}
