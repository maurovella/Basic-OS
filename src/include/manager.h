#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "errors.h"
#include <sys/types.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */


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

/*
    dup_fd
    -------------------------------------
    Description: Duplicates file descriptor
                 deleting the original one
    ** EXITS IF ERROR **
    -------------------------------------
    Parameters:
        fd: File descriptor to be duplicated
        new_fd: Value of the new file descriptor          
    -------------------------------------
    Returns:
        void
*/
void dup_fd (uint32_t fd, uint32_t new_fd);

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
FILE * create_file (uint8_t * file_name, uint8_t * mode);

/*
    is_file
    -------------------------------------
    Description: Checks if path is a regular file
    -------------------------------------
    Parameters:
        path: path to the element to be evaluated
    -------------------------------------
    Returns:
        1 if path is a regular file
        0 otherwise
*/
uint32_t is_file (uint8_t * path);

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

typedef struct shm_info {
    uint8_t * name;
    uint32_t fd;
    void * addr;
    size_t size;
} shm_info;

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
void write_shm (uint32_t fd, void * buf, size_t length, uint32_t pos);

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

typedef struct sem_info { 
    uint8_t * name;
    void * addr;
} sem_info;

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
void close_sem (sem_info * sem)

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
#endif