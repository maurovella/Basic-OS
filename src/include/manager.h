#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "errors.h"

#include <sys/types.h>
#include <stdint.h>

/*
    close_fd
    -------------------------------------
    Description: Closes a file descriptor
    ** EXITS IF ERROR **
    -------------------------------------
    Parameters:
        fd: The file descriptor to close
    -------------------------------------
    Returns:
        void
*/
void close_fd(uint32_t fd);

/*
    create_pipe
    -------------------------------------
    Description: Creates a pipe
    ** EXITS IF ERROR **
    -------------------------------------
    Parameters:
        pipe_fds: The pipe file descriptors
    -------------------------------------
    Returns:
        void
*/
void create_pipe(uint32_t pipe_fds[2]);

#endif