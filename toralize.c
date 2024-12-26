/* toralize.c */

#include "toralize.h"

int main(int argc, char *argv[]) {
    char *host;
    int port, s;
    struct sockaddr_in sock;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <host> <port>\n", argv[0]);
    
        return -1;
    }

    host = argv[1];
    port = atoi(argv[2]);

    // create a socket
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        perror("socket error");

        return -1;
    }

    sock.sin_family = AF_INET;
    sock.sin_port = htons(PROXYPORT);
    sock.sin_addr.s_addr = inet_addr(PROXY);

    // make a connection to the SOCKS4 server
    if (connect(s, (struct sockaddr *)&sock, sizeof(sock))) {
        perror("connect error");

        return -1;
    }

    printf("Connection to the Proxy Server established...!\n");
    close(s);


    return 0;
}