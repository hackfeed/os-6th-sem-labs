#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define BUF_SIZE 20

void *thread_run(void *fs)
{
    int flag = 1;

    char c;
    while (flag == 1)
    {
        flag = fscanf(fs, "%c", &c);
        if (flag == 1)
        {
            fprintf(stdout, "thread2: %c\n", c);
        }
    }

    return NULL;
}

int main()
{
    int fd = open("alphabet.txt", O_RDONLY);
    pthread_t td;

    FILE *fs1 = fdopen(fd, "r");
    char buff1[BUF_SIZE];
    setvbuf(fs1, buff1, _IOFBF, BUF_SIZE);

    FILE *fs2 = fdopen(fd, "r");
    char buff2[BUF_SIZE];
    setvbuf(fs2, buff2, _IOFBF, BUF_SIZE);

    pthread_create(&td, NULL, thread_run, fs2);
    usleep(1);

    int flag = 1;
    char c;
    while (flag == 1)
    {
        flag = fscanf(fs1, "%c", &c);
        if (flag == 1)
        {
            fprintf(stdout, "thread1: %c\n", c);
        }
    }

    pthread_join(td, NULL);

    return 0;
}