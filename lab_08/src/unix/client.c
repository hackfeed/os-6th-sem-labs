#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <errno.h>

#define SOCKET_NAME "/home/hackfeed/socket.soc"
#define BUF_SIZE 256

int main(void)
{
    int sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sfd == -1)
    {
        perror("socket call error");
        return errno;
    }

    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;

    strncpy(addr.sun_path, SOCKET_NAME, strlen(SOCKET_NAME));
    if (connect(sfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        perror("Can't set dest address");
        return errno;
    }

    char msg[BUF_SIZE];
    snprintf(msg, BUF_SIZE, "Message from process %d", getpid());

    if (send(sfd, msg, strlen(msg), 0) == -1)
    {
        perror("send call error");
        return errno;
    }
    else
    {
        printf("send message: %s\n", msg);
    }

    close(sfd);

    return 0;
}
