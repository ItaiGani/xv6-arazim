#include "kernel/stat.h"
#include "kernel/types.h"
#include "user/user.h"

#define MAXARG (32)
#define MAXLINE (512)

int main(int argc, char *argv[]) {
    int argc_dup = argc - 1;
    char *args[MAXARG];
    char buf[MAXLINE];
    char *arg, *c;
    int pid;

    argc_dup = argc - 1;
    for (int i = 0; i < argc - 1; i++)
        args[i] = argv[i + 1];
    arg = buf;
    c = buf;

    while (read(0, c, sizeof(char))) {
        if (*c == '\n') { // we get to the end of a command
            *c = '\0';
            args[argc_dup++] = arg;
            c++;
            arg = c;

            pid = fork();
            if (pid == 0) {
                exec(args[0], args);
            }
            // restart the command
            arg = buf;
            c = buf;
            argc_dup = argc - 1;
        } else if (*c == ' ') {
            *c = '\0';
            args[argc_dup++] = arg;
            c++;
            arg = c;

        } else {
            c++;
        }
    }
    wait(0);
    exit(0);
}