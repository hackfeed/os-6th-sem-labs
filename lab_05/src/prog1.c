#include <fcntl.h>
#include <stdio.h>

#define BUF_SIZE 20

int main()
{
    int fd = open("alphabet.txt", O_RDONLY);

    FILE *fs1 = fdopen(fd, "r");
    char buff1[BUF_SIZE];
    setvbuf(fs1, buff1, _IOFBF, BUF_SIZE);

    FILE *fs2 = fdopen(fd, "r");
    char buff2[BUF_SIZE];
    setvbuf(fs2, buff2, _IOFBF, BUF_SIZE);

    int flag1 = 1, flag2 = 2;
    char c;
    while (flag1 == 1 || flag2 == 1)
    {
        flag1 = fscanf(fs1, "%c", &c);
        if (flag1 == 1)
        {
            fprintf(stdout, "%c", c);
        }
        flag2 = fscanf(fs2, "%c", &c);
        if (flag2 == 1)
        {
            fprintf(stdout, "%c", c);
        }
    }

    fprintf(stdout, "\n");

    return 0;
}