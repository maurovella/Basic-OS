// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "./../include/manager.h"
#include <semaphore.h>

void create_sem (sem_info * sem) {
    if ((sem->addr = sem_open(sem->name, O_CREAT|O_RDWR, S_IRUSR|S_IWUSR, 0)) == SEM_FAILED) {
        perror("Error creating semaphore");
        exit(ERR_CREATING_SEM);
    }
}

void open_sem (sem_info * sem) {
    if ((sem->addr = sem_open(sem->name, O_RDONLY, S_IRUSR, 0)) == SEM_FAILED) {
        perror("Error opening semaphore");
        exit(ERR_OPENING_SEM);
    }
}

void post_sem (sem_info * sem) { 
    if (sem_post(sem->addr) == -1) {
        perror("Error posting semaphore");
        exit(ERR_POSTING_SEM);
    }
}

void wait_sem (sem_info * sem) {
    if (sem_wait(sem->addr) == -1) {
        perror("Error waiting semaphore");
        exit(ERR_WAITING_SEM);
    }
}

void close_sem (sem_info * sem) {
    if (sem_close(sem->addr) == -1) {
        perror("Error closing semaphore");
        exit(ERR_CLOSING_SEM);
    }
}

void unlink_sem (sem_info * sem) {
    if (sem_unlink(sem->name) == -1) {
        perror("Error unlinking semaphore");
        exit(ERR_UNLINKING_SEM);
    }
}
