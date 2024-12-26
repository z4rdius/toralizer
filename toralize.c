/* toralize.c */

#include "toralize.h"

// generate packets for CONNECT
Req *request(struct sockaddr_in *addr) {
    Req *req;

    req = malloc(REQSIZE);

    req->vn = 4;
    req->cd = 1;
    req->dstport = addr->sin_port;
    req->dstip = addr->sin_addr.s_addr;
    strncpy(req->userid, USERNAME, 7);
    
    return req;
}

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {

    int s;
    struct sockaddr_in sock;
    Req *req;
    Res *res;
    char buf[RESSIIZE];
    int success;

    int (*p)(int, const struct sockaddr*, socklen_t);
    p = dlsym(RTLD_NEXT, "connect");

    // create a socket
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0)
    {
        perror("socket error");

        return -1;
    }

    sock.sin_family = AF_INET;
    sock.sin_port = htons(PROXYPORT);
    sock.sin_addr.s_addr = inet_addr(PROXY);

    // make a connection to the SOCKS4 server
    if (p(s, (struct sockaddr *)&sock, sizeof(sock))) {
        perror("connect error");

        return -1;
    }

    printf("Connection to the Proxy Server established...!\n");

    req = request((struct sockaddr_in *)addr);
    write(s, req, REQSIZE);

    memset(buf, 0, RESSIIZE);
    if (read(s, buf, RESSIIZE) < 1)
    {
        perror("read error");
        free(req);
        close(s);

        return -1;
    }

    res = (Res *)buf;

    success = (res->cd == 90);
    if (!success)
    {
        fprintf(stderr, "Unable to traverse the proxy, error code: %d\n", res->cd);

        close(s);
        free(req);

        return -1;
    }

    printf("Connected trough the proxy successfully...!\n");

    dup2(s, sockfd);

    free(req);

    return 0;
}