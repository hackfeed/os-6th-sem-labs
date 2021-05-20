#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void *thread_run(void *fs)
{
    int fd = *(int *)fs;
    int flag = 1;

    char c;
    while (flag == 1)
    {
        flag = read(fd, &c, 1);
        if (flag == 1)
        {
            write(1, &c, 1);
        }
    }

    return NULL;
}

int main()
{
    int fd1 = open("alphabet.txt", O_RDONLY);
    int fd2 = open("alphabet.txt", O_RDONLY);
    int rc = 1;

    pthread_t td;
    pthread_create(&td, NULL, thread_run, &fd2);
    usleep(1);

    char c;
    while (rc == 1)
    {
        rc = read(fd1, &c, 1);
        if (rc == 1)
        {
            write(1, &c, 1);
        }
    }

    pthread_join(td, NULL);
    fprintf(stdout, "\n");

    return 0;
}