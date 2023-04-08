#include "./../include/manager.h"
#include <dirent.h>


FILE * create_file (uint8_t * file_name, uint8_t * mode) {
    
    FILE * f = fopen(file_name, mode);
    
    if (f == NULL) {
        perror("Error creating file.");
        exit(ERR_CREATING_FILE); 
    }
    
    return f;
}

uint32_t is_file (uint8_t * path) {
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

void close_file (FILE * file) {
    if (fclose(file) == -1) {
        perror("Error closing file.");
        exit(ERR_CLOSE_FILE);
    }
}