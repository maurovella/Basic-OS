#include <stdio.h>
#include <string.h>

int main(int argc, char * argv[])
{
    char arg1[256], arg2[256], arg3[256];
    int arg_length[3];

    fgets(arg1, 256, stdin);
    fgets(arg2, 256, stdin);
    fgets(arg3, 256, stdin);

    arg_length[0] = strlen(arg1);
    arg_length[1] = strlen(arg2);
    arg_length[2] = strlen(arg3);

    arg1[arg_length[0] - 1] = 0;
    arg2[arg_length[1] - 1] = 0;
    arg3[arg_length[2] - 1] = 0;
    printf("%s\n", arg1);
    printf("%s\n", arg2);
    printf("%s\n", arg3);
    return 0;
}
