#include "./../include/manager.h"

FILE * create_file (char * file_name, char * mode) {
    
    FILE * f = fopen(file_name, mode);
    
    if (f == NULL) {
        perror("Error creating file.");
        exit(ERR_CREATING_FILE); 
    }
    
    return f;
}

int is_file (char * path) {
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