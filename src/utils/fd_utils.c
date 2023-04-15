// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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

int dup_fd (int fd, int new_fd) {
    if (dup2(fd, new_fd) == -1) {
        perror("Error duplicating file descriptor.");
        exit(ERR_DUPLICATING_FD);
    }
    close_fd(fd);
    return new_fd;
}

void select_fd (int nfds, fd_set * read_fds, fd_set * write_fds, fd_set * error_fds, struct timeval *timeout) {
    if (select(nfds, read_fds, NULL, NULL, NULL) == -1) {
        perror("Error selecting available file descriptors");
        exit(ERR_SELECTING_FD);
    }
}


void read_fd (int fd, void * buf, size_t count) {
    if (read(fd, buf, count) == -1) {
        perror("Error reading from fd");
        exit(ERR_READING_FD);
    }
}

void write_fd (int fd, void * buf, size_t count) {
    if (write(fd, buf, count) == -1) {
        perror("Error writing to fd");
        exit(ERR_WRITING_FD);
    }
}