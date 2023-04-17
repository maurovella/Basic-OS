#ifndef _DEFS_H_
#define _DEFS_H_

#include <unistd.h>

#define READ 0
#define WRITE 1
#define ORIGINAL 0
#define BACKUP 1
#define INITIAL_FILES_PER_SLAVE 3
#define SLAVES_FROM_FILES_FACTOR 9
#define SLAVES_FROM_FILES(cant_files) (((cant_files) / SLAVES_FROM_FILES_FACTOR) + 1)
#define MAX_SLAVES 50
#define MAX_LEN 256
#define MIN(a, b) ((a) <= (b) ? (a) : (b))
#define MD5_SIZE 32
#define SHM_SIZE 16834
#define SHM_NAME "/shm"
#define SEM_NAME "/reading_sem"

typedef struct md5_info {
    char hash[MD5_SIZE + 1];
    pid_t pid;
    char file_name[MAX_LEN];
    int files_left;
} md5_info;

typedef struct shm_info {
    char * name;
    int fd;
    void * addr;
} shm_info;

typedef struct sem_info { 
    char * name;
    void * addr;
} sem_info;

#endif
