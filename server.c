#include "server.h"
#include "stdio.h"
#include "stdlib.h"

struct Server construct_server(int domain, int type, int protocol, int port, in_addr_t ip_address, int backlog, void(*launch)())
{
    struct Server server;
    server.domain = domain;
    server.type = type;
    server.protocol = protocol;
    server.port = port;
    server.ip_address = ip_address;
    server.backlog = backlog;

    server.launch = launch;

    server.address.sin_family = domain;
    server.address.sin_port = htons(port); /* Convert from host to network byte order */
    server.address.sin_addr.s_addr = htonl(ip_address);

    server.sockfd = socket(domain, type, protocol);
    if (server.sockfd == -1)
    {
        perror("Failed to create a socket!");
        exit(1);
    }

    int bind_res = bind(server.sockfd, (struct sockaddr*)&server.address, sizeof(server.address));
    if(bind_res == -1)
    {
        perror("Failed to bind a socket!");
        exit(1);
    }

    int listen_res = listen(server.sockfd, backlog);
    if(listen_res == -1)
    {
        perror("Failed to prepare a socket to accept connections!");
        exit(1);
    }

    return server;
}
