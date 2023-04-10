#include "./../include/manager.h"

void close_fd (int fd) {
    if (close(fd) == -1) {
        perror("Error closing file descriptor.");
        exit(ERR_CLOSING_FD);
    }
}


void create_pipe (int pipe_fds[2]) {
    if (pipe(pipe_fds) == -1) {
            perror("Error creating pipe.");
            exit(ERR_CREATING_PIPE); 
    }
}

void dup_fd (int fd, int new_fd) {
    if (dup2(fd, new_fd) == -1) {
        perror("Error duplicating file descriptor.");
        exit(ERR_DUPLICATING_FD);
    }
}