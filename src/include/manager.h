#ifndef _MANAGER_H_
#define _MANAGER_H_

#define __DARWIN_C_LEVEL 200809L
#define _XOPEN_SOURCE 500

#include "errors.h"
#include "defs.h"
#include <sys/types.h>
#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <sys/wait.h>
#include <sys/select.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

/* -----  FILE DESCRIPTOR FUNCTIONS  ----- */

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
void close_fd(int fd);

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
void create_pipe(int pipe_fds[2]);

/*
    dup_fd
    -------------------------------------
    Description: Duplicates file descriptor
                 closing the original one
    ** EXITS IF ERROR **
    -------------------------------------
    Parameters:
        fd: File descriptor to be duplicated
        new_fd: Value of the new file descriptor          
    -------------------------------------
    Returns:
        new_fd
*/
int dup_fd (int fd, int new_fd);

/*
    select_fd
    -------------------------------------
    Description: Using select(), selects fd 
                 ready to make I/O operation 
                 such as open() & read().
    ** EXITS IF ERROR **
    -------------------------------------
    Parameters:
        nfds: Amount of file descriptors to 
              be checked in each set.
        read_fds: File descriptor set of 
                  exclusively read file descriptors.
        write_fds: File descriptor set of 
                   exclusively write file descriptors.
        error_fds: File descriptor set of 
                   exclusively error file descriptors.
        timeout: specifies the interval that 
                 select() should block waiting 
                 for a file descriptor to become ready.  
    -------------------------------------
    Returns:
        void
*/
void select_fd (int nfds, fd_set *read_fds, fd_set *write_fds, fd_set *error_fds, struct timeval *timeout);

/*
    read_fd 
    -------------------------------------
    Description: Reads count bytes from fd and saves it on buf
    ** EXITS IF ERROR **
    -------------------------------------
    Parameters:
        fd: File descriptor to be read from
        buf: Buffer where the information will be stored
        count: Amount of bytes to read    
    -------------------------------------
    Returns:
        void
*/
void read_fd (int fd, void *buf, size_t count);

/*
    write_fd 
    -------------------------------------
    Description: Writes count bytes from buf in fd
    ** EXITS IF ERROR **
    -------------------------------------
    Parameters:
        fd: File descriptor to be written 
        buf: Buffer from which the information will be written
        count: Amount of bytes to write    
    -------------------------------------
    Returns:
        void
*/
void write_fd (int fd, void * buf, size_t count);

/* -----  FILE FUNCTIONS  ----- */

/*
    create_file
    -------------------------------------
    Description: Creates an empty file
    ** EXITS IF ERROR **
    -------------------------------------
    Parameters:
        file_name: Name of the file
        mode: Mode in which the file is opened
    -------------------------------------
    Returns:
        FILE pointer to the new file
*/
FILE * create_file (char * file_name, char * mode);

/*
    is_file
    -------------------------------------
    Description: Checks if path is a regular file
    -------------------------------------
    Parameters:
        path: Path to the element to be evaluated
    -------------------------------------
    Returns:
        1 if path is a regular file
        0 otherwise
*/
int is_file (char * path);

/*
    close_file
    -------------------------------------
    Description: Closes file
    ** EXITS IF ERROR **
    -------------------------------------
    Parameters:
        file: Pointer to the file to be closed
    -------------------------------------
    Returns:
        void
*/
void close_file (FILE * file);

/* -----  SHARED MEMORY FUNCTIONS  ----- */

/*
    create_shm
    -------------------------------------
    Description: Creates shared memory
    ** EXITS IF ERROR **
    -------------------------------------
    Parameters:
        shm: Pointer to the shared memory structure
    -------------------------------------
    Returns:
        void
*/
void create_shm (shm_info * shm);

/*
    open_shm
    -------------------------------------
    Description: Opens shared memory
    ** EXITS IF ERROR **
    -------------------------------------
    Parameters:
        shm: Pointer to the shared memory structure
    -------------------------------------
    Returns:
        void
*/
void open_shm (shm_info * shm);

/*
    write_shm
    -------------------------------------
    Description: Writes buf in the desired file descriptor
                 (in this implementation it will only be used
                 to write in shared memory)
    ** EXITS IF ERROR **
    -------------------------------------
    Parameters:
        fd: File descriptor to be written
        buf: Buffer to be written
        length: Size of the buffer
        pos: Position within the file descriptor where buf
             will be written
    -------------------------------------
    Returns:
        void
*/
void write_shm (int fd, void * buf, size_t length, int pos);

/*
    close_shm
    -------------------------------------
    Description: Closes shared memory
    ** EXITS IF ERROR **
    -------------------------------------
    Parameters:
        shm: Pointer to the shared memory structure
    -------------------------------------
    Returns:
        void
*/
void close_shm (shm_info * shm);

/*
    unlink_shm
    -------------------------------------
    Description: Unlinks shared memory
    ** EXITS IF ERROR **
    -------------------------------------
    Parameters:
        shm: Pointer to the shared memory structure
    -------------------------------------
    Returns:
        void
*/
void unlink_shm (shm_info * shm);

/* ----- SEMAPHORE FUNCTIONS ----- */

/*
    create_sem
    -------------------------------------
    Description: Creates a semaphore in decreased state
    ** EXITS IF ERROR **
    -------------------------------------
    Parameters:
        sem: Pointer to the semaphore structure
    -------------------------------------
    Returns:
        void
*/
void create_sem (sem_info * sem);

/*
    open_sem
    -------------------------------------
    Description: Creates a semaphore in decreased state (0)
    ** EXITS IF ERROR **
    -------------------------------------
    Parameters:
        sem: Pointer to the semaphore structure
    -------------------------------------
    Returns:
        void
*/
void open_sem (sem_info * sem);

/*
    post_sem
    -------------------------------------
    Description: Increments semaphore value
    ** EXITS IF ERROR **
    -------------------------------------
    Parameters:
        sem: Pointer to the semaphore structure
    -------------------------------------
    Returns:
        void
*/
void post_sem (sem_info * sem);

/*
    wait_sem
    -------------------------------------
    Description: Decrements semaphore value
    ** EXITS IF ERROR **
    -------------------------------------
    Parameters:
        sem: Pointer to the semaphore structure
    -------------------------------------
    Returns:
        void
*/
void wait_sem (sem_info * sem);

/*
    close_sem
    -------------------------------------
    Description: Closes semaphore
    ** EXITS IF ERROR **
    -------------------------------------
    Parameters:
        sem: Pointer to the semaphore structure
    -------------------------------------
    Returns:
        void
*/
void close_sem (sem_info * sem);

/*
    unlink_sem
    -------------------------------------
    Description: Destoys semaphore once it is closed in all processes usiing it.
    ** EXITS IF ERROR **
    -------------------------------------
    Parameters:
        sem: Pointer to the semaphore structure
    -------------------------------------
    Returns:
        void
*/
void unlink_sem (sem_info * sem);

/* ----- PROCESS FUNCTIONS ----- */

/*
    create_slave
    -------------------------------------
    Description: Creates a slave process using fork()
    ** EXITS IF ERROR **
    -------------------------------------
    Parameters:
        void
    -------------------------------------
    Returns:
        pid of the slave process 

*/
pid_t create_slave();

/*
    kill_slave
    -------------------------------------
    Description: Kills a slave process using kill()
                 with the signal SIGKILL
    ** EXITS IF ERROR **
    -------------------------------------
    Parameters:
        pid: Pid of the slave process to be killed
    -------------------------------------
    Returns:
        void

*/
void kill_slave(pid_t pid);

/*
    slave
    -------------------------------------
    Description: Creates a sub-slave in order
                 to calculate the md5 hash of
                 file present in the app_to_slave pipe,
                 then it writes the result in the 
                 slave_to_app pipe
    ** EXITS IF ERROR **
    -------------------------------------
    Parameters:
        app_to_slave: File descriptors of the app_to_slave pipe
        slave_to_app: File descriptors of the slave_to_app pipe
    -------------------------------------
    Returns:
        0 on completion
        exits if there is an error

*/
int slave (int * app_to_slave, int * slave_to_app);

#endif
