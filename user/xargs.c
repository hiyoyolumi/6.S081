#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAXLEN 128

void copy(char **dest, char *src)
{
	*dest = malloc(strlen(src) + 1);
	strcpy(*dest, src);
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(2, "xargs: Too few parameters\n");
		exit(1);
	}

	while (1) {
		/* Get one argv for a command to execute */
		char c;
		int flag = 0;
		int cnt = 0;
		int new_argc = 0;
		int offset = 0;
		char tmp[MAXARG][MAXLEN];
		char *new_argv[MAXARG];
		while ((read(0, &c, 1) == 1) && c != '\n') {
			if (c == ' ' && flag == 1) {
				cnt++;
				offset = 0;
				flag = 0;
			} else if (c != ' ') {
				tmp[cnt][offset++] = c;
				new_argc = cnt;
				flag = 1;
			}
		}
		for (int i = 1; i < MAXARG; i++) {
			copy(&new_argv[i - 1], tmp[i]);
		}
		new_argc++;
		new_argv[new_argc] = 0;
		if (fork() == 0) {
			exec(argv[1], new_argv);
		} else {
			wait(0);
		}
	}
}