#include "./../include/manager.h"
#include <sys/mman.h>

void create_shm (shm_info * shm) {
    if ((shm->fd = shm_open(shm->name, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR)) == -1) {
        perror("Error creating shared memory.");
        exit(ERR_CREATING_SHM);
    }

    if (ftruncate(shm->fd, shm->size) == -1) {
        perror("Error truncating shared memory.");
        exit(ERR_TRUNCATING_SHM);
    }

    if ((shm->addr = mmap(NULL, shm->size, PROT_READ|PROT_WRITE, MAP_SHARED, shm->fd, 0)) == (void *) -1) {
        perror("Error mapping shared memory.");
        exit(ERR_MAPPING_SHM);
    }
}

void open_shm (shm_info * shm) {
    if ((shm->fd = shm_open(shm->name, O_RDONLY, S_IRUSR)) == -1) {
        perror("Error opening shared memory.");
        exit(ERR_OPENING_SHM);
    }

    if ((shm->addr = mmap(NULL, shm->size, PROT_READ, MAP_SHARED, shm->fd, 0)) == (void *) -1) {
        perror("Error mapping shared memory.");
        exit(ERR_MAPPING_SHM);
    }
}

void write_shm (int fd, void * buf, size_t length, int pos) {
    if (pwrite(fd, buf, length, length * pos) == -1) {
        perror("Error writing shared memory.");
        exit(ERR_WRITING_SHM);
    }
}

void close_shm (shm_info * shm) {
    if (munmap(shm->addr, shm->size) == -1) {
        perror("Error unmapping shared memory.");
        exit(ERR_UNMAPPING_SHM);
    }

    if (close(shm->fd) == -1) {
        perror("Error closing shared memory.");
        exit(ERR_CLOSING_SHM);
    }
}

void unlink_shm (shm_info * shm) { 
    if (shm_unlink(shm->name) == -1) {
        perror("Error unlinking shared memory.");
        exit(ERR_UNLINKING_SHM);
    }
}