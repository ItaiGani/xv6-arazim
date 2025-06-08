#include "kernel/stat.h"
#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {

    int ptoc[2];
    int ctop[2];
    pipe(ptoc);
    pipe(ctop);

    // child
    if (fork() == 0) {
        close(ptoc[1]);
        close(ctop[0]);

        // reading the parent ping and printing it
        char ping[1];
        read(ptoc[0], ping, 1);
        printf("%d: received ping\n", getpid());

        write(ctop[1], "1", 1);

        close(ptoc[0]);
        close(ctop[1]);
    }
    // parent
    else {
        close(ptoc[0]);
        close(ctop[1]);

        write(ptoc[1], "1", 1);

        // reading the child pong and printing it
        char pong[1];
        read(ctop[0], pong, 1);
        printf("%d: received pong\n", getpid());

        close(ctop[0]);
        close(ptoc[1]);
    }
}
