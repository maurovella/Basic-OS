// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "./../include/manager.h"
#include <signal.h>

pid_t create_slave() {
    pid_t pid = fork();
    if (pid == -1) {
        perror("Error creating slave");
        exit(ERR_CREATING_SLAVE);
    }
    return pid;
}

void kill_slave(pid_t pid) {
    if (kill(pid, SIGKILL) == -1) {
        perror("Error killing process");
        exit(ERR_KILLING_PROCESS);
    }
}