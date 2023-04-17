// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/manager.h"

int main(int argc, char * argv[])
{
    char shm_name[MAX_LEN] = {0}, reading_sem_name[MAX_LEN] = {0};
    
    if (argc != 3) { 
        // Shm and sem info was sent through stdin
        fgets(shm_name, MAX_LEN, stdin);
        fgets(reading_sem_name, MAX_LEN, stdin);
        
        // Replacing newline caracter with '\0'
        shm_name[strcspn(shm_name, "\n")] = '\0'; 
        reading_sem_name[strcspn(reading_sem_name, "\n")] = '\0'; 
    
    } else {

        if (strlen(argv[1]) >= MAX_LEN || strlen(argv[2]) >= MAX_LEN) {
            perror("Argument length is too long");
            exit(ERR_INVALID_LENGTH);
        }

        // Shm and sem info was sent through argv
        strcpy(shm_name, argv[1]);
        strcpy(reading_sem_name, argv[2]);
    }

    shm_info shm;
    sem_info reading_sem;
    
    shm.name = shm_name;
    reading_sem.name = reading_sem_name;

    // Creating shared memory and semaphore
    open_shm(&shm);
    open_sem(&reading_sem);

    md5_info md5;

    int finished = 0;
    for (int i = 0; !finished; i++) {

        // Waiting to read resources
        wait_sem(&reading_sem);

        // Reading from shared memory
        pread(shm.fd, &md5, sizeof(md5_info), i * sizeof(md5_info));
        printf("MD5: %s -- NAME: %s -- PID: %d\n", md5.hash, md5.file_name, md5.pid);

        if (md5.files_left <= 1) {
            finished = 1;
        }
    }

    // Signal application process that all resources have been read and that they can now be destroyed
    post_sem(&reading_sem);

    // Closing shared memory and semaphore
    close_shm(&shm);
    close_sem(&reading_sem);

    return 0;
}
