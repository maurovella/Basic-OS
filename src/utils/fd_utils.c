#include "./../include/manager.h"

void close_fd (uint32_t fd) {
    if (close(fd) == -1) {
        perror("Error closing file descriptor");
        exit(ERR_CLOSE_FD);
    }
}


void create_pipe (uint32_t pipe_fds[2]) {
    if (pipe(pipe_fds) == -1) {
            perror("Error creating pipe");
            exit(ERR_PIPE); 
    }
}

void dup_fd (uint32_t fd, uint32_t new_fd) {
    if (dup2(fd, new_fd) == -1) {
        perror("Error duplicating file descriptor");
        exit(ERR_DUP_FD);
    }
}