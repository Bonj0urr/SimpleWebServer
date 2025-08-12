#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <netinet/in.h>

struct Server
{
    int domain;
    int type;
    int protocol;
    int port;
    in_addr_t ip_address;
    int backlog;
    int sockfd;
    struct sockaddr_in address;

    void(*launch)(struct Server*);
};

struct Server construct_server(int domain, int type, int protocol, int port, in_addr_t ip_address, int backlog, void(*launch)());


#endif /* SERVER_H */
