#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    int fd1 = open("alphabet.txt", O_RDONLY);
    int fd2 = open("alphabet.txt", O_RDONLY);
    int rc1 = 1, rc2 = 1;

    char c;
    while (rc1 == 1 || rc2 == 1)
    {
        rc1 = read(fd1, &c, 1);
        if (rc1 == 1)
        {
            write(1, &c, 1);
        }
        rc2 = read(fd2, &c, 1);
        if (rc2 == 1)
        {
            write(1, &c, 1);
        }
    }

    fprintf(stdout, "\n");

    return 0;
}