#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
        int s;

        if (argc != 2) {
                fprintf(2, "Too few parameters\n");
                exit(1);
        }

        s = atoi(argv[1]);
        sleep(s);

        exit(0);
}