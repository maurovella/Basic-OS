#include "./../include/manager.h"
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>


FILE * create_output_file (char * file_name, char * mode) {
    
    FILE * output = fopen(file_name, mode);
    
    if (output == NULL) {
        perror("Error creating output file");
        exit(ERR_CREATING_FILE); 
    }
    
    return output;
}

uint32_t is_file (char * path) {
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

void close_file (FILE * file) {
    if (fclose(file) == -1) {
        perror("Error closing file");
        exit(ERR_CLOSE_FILE);
    }
}