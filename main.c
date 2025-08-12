#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include "string.h"
#include <fcntl.h>
#include <sys/sendfile.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 256
#define PORT 8080

void launch_server(struct Server* server);

int main()
{
    int backlog = 9;
    struct Server server = construct_server(AF_INET, SOCK_STREAM, 0, PORT, INADDR_ANY, backlog, launch_server);

    server.launch(&server);

    return 0;
}

void launch_server(struct Server* server)
{
    if(!server)
    {
        perror("Server isn't valid!");
        exit(1);
    }
    printf("Waiting for the connection...");

    unsigned int address_length = sizeof(server->address);
    int client_sockfd = accept(server->sockfd, (struct sockaddr*)&server->address, &address_length);
    if(client_sockfd == -1)
    {
        perror("Failed to connect a client!");
        exit(1);
    }

    char buffer[BUFFER_SIZE] = {0};

    int read_res = recv(client_sockfd, buffer, BUFFER_SIZE - 1, 0);
    if(read_res == -1)
    {
        perror("Failed to read from the client socket!");
        exit(1);
    }

    /* Extract the requested file path */
    char* requested_file = buffer + 5;
    *strchr(requested_file, ' ') = '\0';

    int requested_fd = open(requested_file, O_RDONLY);
    if(requested_fd == -1)
    {
        perror("Failed to open a requested file!");
        exit(1);
    }

    int send_res = sendfile(client_sockfd, requested_fd, 0, BUFFER_SIZE - 1);
    if(send_res == -1)
    {
        perror("Failed to transfer the requested file data!");
        exit(1);
    }

    close(server->sockfd);
    close(client_sockfd);
    close(requested_fd);
}
