#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>

void *thread_run(void *fs)
{
    struct stat info;

    for (char c = 'a'; c <= 'z'; c += 2)
    {
        fprintf(fs, "%c", c);
    }
    fclose(fs);
    stat("outt.txt", &info);
    fprintf(stdout, "fclose outt.txt in second thread: inode is %ld, filesize is %ld\n", info.st_ino, info.st_size);

    return NULL;
}

int main()
{
    struct stat info;

    FILE *fd1 = fopen("outt.txt", "w");
    stat("outt.txt", &info);
    fprintf(stdout, "first fopen outt.txt in main thread: inode is %ld, filesize is %ld\n", info.st_ino, info.st_size);

    FILE *fd2 = fopen("outt.txt", "w");
    stat("outt.txt", &info);
    fprintf(stdout, "second fopen outt.txt in main thread: inode is %ld, filesize is %ld\n", info.st_ino, info.st_size);

    pthread_t td;
    pthread_create(&td, NULL, thread_run, fd2);

    for (char c = 'b'; c <= 'z'; c += 2)
    {
        fprintf(fd1, "%c", c);
    }
    fclose(fd1);
    stat("outt.txt", &info);
    fprintf(stdout, "fclose outt.txt in main thread: inode is %ld, filesize is %ld\n", info.st_ino, info.st_size);
    pthread_join(td, NULL);

    return 0;
}