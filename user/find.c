#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  buf[strlen(p)] = '\0';
  return buf;
}

void display(char *path, char *findstr)
{
        int fd;
        char buf[512], *p, *tmp;
        struct stat st;
        struct dirent de;

        if ((fd = open(path, 0)) < 0) {
                fprintf(2, "find: cannot open %s\n", path);
                return;
        }
        if (fstat(fd, &st) < 0) {
                fprintf(2, "find: cannot stat %s\n", path);
                close(fd);
                return;
        }
        switch (st.type) {
        case T_FILE:
                if (strcmp(fmtname(path), findstr) == 0)
                        printf("%s\n", path);
                break;
        case T_DIR:
                if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
                        printf("display: path too long\n");
                        break;
                }
                strcpy(buf, path);
                p = buf+strlen(buf);
                *p++ = '/';
                tmp = p;
                while (read(fd, &de, sizeof(de)) == sizeof(de)) {
                        if (de.inum == 0)
                                continue;
                        if (!strcmp(de.name, ".") || !strcmp(de.name, ".."))
                                continue;
                        p = tmp;
                        int len = strlen(de.name);
                        memmove(p, de.name, len);
                        p[len] = '\0';
                        display(buf, findstr);
                }
                break;
        }
        close(fd);
}

int main(int argc, char *argv[])
{
        if (argc == 1) {
                fprintf(2, "find: Too few parameters\n");
                exit(1);
        } else if (argc == 2) {
                display(".", argv[1]);
        } else if (argc == 3) {
                display(argv[1], argv[2]);
        }
        exit(0);
}