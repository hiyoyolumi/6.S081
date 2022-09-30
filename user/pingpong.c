#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
        int fd[2];
        int pid;

        if (pipe(fd) < 0) {
                fprintf(2, "pipe error");
                exit(1);
        }

        if ((pid = fork()) < 0) {
                fprintf(2, "fork error");
                exit(1);
        } else if (pid == 0) {
                char buf[2];
                if (read(fd[0], buf, 1) != 1) {
                        fprintf(2, "child read failed\n");
                        exit(1);
                }
                close(fd[0]);
                printf("%d: received ping\n", getpid());
                if (write(fd[1], buf, 1) != 1) {
                        fprintf(2, "child write failed\n");
                        exit(1);
                }
                close(fd[1]);
                exit(0);
        } else {
                char buf[2] = "p";
                if (write(fd[1], buf, 1) != 1) {
                        fprintf(2, "parent write failed\n");
                        exit(1);
                }
                close(fd[1]);
                if (read(fd[0], buf, 1) != 1) {
                        fprintf(2, "parent read failed\n");
                        exit(1);
                }
                close(fd[0]);
                printf("%d: received pong\n", getpid());
                exit(0);
        }
        exit(0);
}