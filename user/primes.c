#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void new_pipe(int fd[2])
{
        int prime;
        int n;
        int pid;
        int new_fd[2];
        int flag;

        close(fd[1]);

        if (read(fd[0], &prime, 4) != 4) {
                fprintf(2, "read error\n");
                exit(1);
        }
        printf("prime %d\n", prime);

        flag = read(fd[0], &n, 4);
        if (flag) {
                if (pipe(new_fd) < 0) {
                        fprintf(2, "pipe error\n");
                        exit(1);
                }
                if ((pid = fork()) < 0) {
                        fprintf(2, "fork error\n");
                        exit(1);
                } else if (pid == 0) {
                        new_pipe(new_fd);
                } else {
                        close(new_fd[0]);
                        if (n % prime) {
                                if (write(new_fd[1], &n, 4) != 4) {
                                        fprintf(2, "write error\n");
                                        exit(1);
                                }
                        }
                        while (read(fd[0], &n, 4)) {
                                if (n % prime) {
                                        if (write(new_fd[1], &n, 4) != 4) {
                                                fprintf(2, "write error\n");
                                                exit(1);
                                        }
                                }
                        }
                        close(fd[0]);
                        close(new_fd[1]);
                        wait(0);
                        exit(0);
                }
        }
        exit(0);
}

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
                new_pipe(fd);
        } else {
                close(fd[0]);
                for (int i = 2; i <= 35; i++) {
                        if (write(fd[1], &i, 4) != 4) {
                                fprintf(2, "write error\n");
                                exit(1);
                        }
                }
                close(fd[1]);
                wait(0);
                exit(0);
        }
        exit(0);
}