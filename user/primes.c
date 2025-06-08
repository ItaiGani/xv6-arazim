#include "kernel/stat.h"
#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int pid = 0;
    int next_num;
    int first_num = 2;
    int p[2];

    pipe(p);
    for (int i = 2; i <= 35; i++) {
        write(p[1], &i, sizeof(int));
    }
    close(p[1]);
    printf("prime 2\n");

    while (read(p[0], &next_num, sizeof(int))) {
        if (next_num % first_num != 0) {
            if (pid == 0) {
                int g = p[0];
                pipe(p);
                pid = fork();
                if (pid) {
                    close(p[0]);
                    p[0] = g;
                } else {
                    close(p[1]);
                    first_num = next_num;
                    printf("prime %d\n", first_num);
                }
            }
            write(p[1], &next_num, sizeof(int));
        }
    }
    close(p[0]);
    close(p[1]);
    wait(0);
}
