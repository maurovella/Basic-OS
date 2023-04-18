// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "./include/manager.h"

typedef struct sub_slave_info {
    int pipe_fd[2];     // File descriptors connecting sub_slave to slave 
    fd_set sets_fd[2];  // File descritpor sets (original = 0 and backup = 1)
} sub_slave_info;

int finished = 0;

int slave (int * app_to_slave, int * slave_to_app) {

    // Second arg = file name -> read from app
    char * args[] = {"md5sum", NULL, NULL}; 
    char * file_name;
    char result[MAX_LEN] = {0};

    // Closing unused pipes
    close_fd(app_to_slave[WRITE]);
    close_fd(slave_to_app[READ]);
    
    fd_set app_to_slave_set[2];

    // Initializes the set on NULL
    FD_ZERO(&(app_to_slave_set[ORIGINAL]));

    // Includes fd in fd_set 
    FD_SET(app_to_slave[READ], &(app_to_slave_set[ORIGINAL]));

    // Backup original set
    app_to_slave_set[BACKUP] = app_to_slave_set[ORIGINAL];

    sub_slave_info sub_slave;

    create_pipe(sub_slave.pipe_fd);

    // Redirecting sub_slave file descriptors to stdin and stdout
    sub_slave.pipe_fd[READ] = dup_fd(sub_slave.pipe_fd[READ], 0);
    sub_slave.pipe_fd[WRITE] = dup_fd(sub_slave.pipe_fd[WRITE], 1);

    // Initializes the set on NULL
    FD_ZERO(&(sub_slave.sets_fd[ORIGINAL]));

    // Includes fd in fd_set 
    FD_SET(sub_slave.pipe_fd[READ], &(sub_slave.sets_fd[ORIGINAL]));

    // Backup original set
    sub_slave.sets_fd[BACKUP] = sub_slave.sets_fd[ORIGINAL];

    while (!finished) {
        
        // Wait for sub_slave to finish with select_fd
        select_fd(FD_SETSIZE, &(app_to_slave_set[ORIGINAL]), NULL, NULL, NULL);

        // Going back to the original state
        app_to_slave_set[ORIGINAL] = app_to_slave_set[BACKUP];

        // Reading file_name from app
        read_fd(app_to_slave[READ], &file_name, sizeof(char *)); 

        if (create_slave() == 0) {
            // Sub_slave process
            args[1] = file_name;
            execvp("md5sum", args);
        } else {
            // Slave process
            // md5sum result = MD5 + 1 (" ") + strlen(file_name) + 1 (\0)
            int result_len = MD5_SIZE + 1 + strlen(file_name) + 1;

            // Read md5sum result from sub_slave
            read_fd(sub_slave.pipe_fd[READ], result, result_len * sizeof(char));
            result[result_len] = 0;
        
            // Flush stdin
			int dump;
			while ((dump = getchar()) != '\n' && dump != EOF) {;}

            // Wait for sub_slave to finish
            wait(NULL);

            // Writing result on app
            write_fd(slave_to_app[WRITE], result, MAX_LEN * sizeof(char));
        }
        
    }
    
    return 0;    
}
