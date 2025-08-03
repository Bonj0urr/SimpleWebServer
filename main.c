#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "string.h"
#include <fcntl.h>
#include <sys/sendfile.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 256
#define PORT 8080

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("Failed to create a socket!");
        return 1;
    }

    struct sockaddr_in addr = { AF_INET, htons(PORT), 0 }; /* Convert from host to network byte order */
    addr.sin_addr.s_addr = inet_addr("127.0.0.1"); /* Server's IP address */

    int bind_res = bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    if(bind_res == -1)
    {
        printf("Failed to bind a socket!");
        return 1;
    }

    int listen_res = listen(sockfd, 9);
    if(listen_res == -1)
    {
        printf("Failed to prepare a socket to accept connections!");
        return 1;
    }

    int client_sockfd = accept(sockfd, 0, 0);
    if(client_sockfd == -1)
    {
        printf("Failed to connect a client!");
        return 1;
    }

    char buffer[BUFFER_SIZE] = {0};

    int read_res = recv(client_sockfd, buffer, BUFFER_SIZE - 1, 0);
    if(read_res == -1)
    {
        printf("Failed to read from the client socket!");
        return 1;
    }

    /* Extract the requested file path */
    char* requested_file = buffer + 5;
    *strchr(requested_file, ' ') = '\0';

    int requested_fd = open(requested_file, O_RDONLY);
    if(requested_fd == -1)
    {
        printf("Failed to open a requested file!");
        return 1;
    }

    int send_res = sendfile(client_sockfd, requested_fd, 0, BUFFER_SIZE - 1);
    if(send_res == -1)
    {
        printf("Failed to transfer the requested file data!");
        return 1;
    }

    close(sockfd);
    close(client_sockfd);
    close(requested_fd);

    return 0;
}
