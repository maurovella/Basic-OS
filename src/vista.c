// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/manager.h"

/* TEST --------*/

int main(int argc, char * argv[])
{
    char shm_name[256] = {0}, reading_sem_name[256] = {0}, closing_sem_name[256] = {0};
    
    fgets(shm_name, 256, stdin);
    fgets(reading_sem_name, 256, stdin);
    fgets(closing_sem_name, 256, stdin);

    shm_name[strcspn(shm_name, "\n")] = '\0'; // Elimina el carácter de nueva línea de shm_name
    reading_sem_name[strcspn(reading_sem_name, "\n")] = '\0'; // Elimina el carácter de nueva línea de sem_name
    closing_sem_name[strcspn(closing_sem_name, "\n")] = '\0';

    shm_info shm;
    sem_info reading_sem;
    sem_info closing_sem;
    
    shm.name = shm_name;
    reading_sem.name = reading_sem_name;
    closing_sem.name = closing_sem_name;

    open_shm(&shm);
    open_sem(&reading_sem);
    open_sem(&closing_sem);

    wait_sem(&closing_sem);
    md5_info md5;

    int finished = 0;
    for (int i = 0; !finished; i++) {
        wait_sem(&reading_sem);
        pread(shm.fd, &md5, sizeof(md5_info), i * sizeof(md5_info));
        printf("file_name: %s -- hash: %s -- pid: %d\n", md5.file_name, md5.hash, md5.pid);

        if (md5.files_left <= 1) {
            finished = 1;
        }
    }
    post_sem(&closing_sem);
    close_shm(&shm);
    close_sem(&reading_sem);
    close_sem(&closing_sem);

    return 0;
}

/* -------- TEST */