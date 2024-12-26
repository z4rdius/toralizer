/* toralize.c */

#include "toralize.h"

Req *request(const char *dstip, const int dstport) {
    Req *req;

    req = malloc(REQSIZE);

    req->vn = 4;
    req->cd = 1;
    req->dstport = htons(dstport);
    req->dstip = inet_addr(dstip);
    strncpy(req->userid, USERNAME, 7);
    
    return req;
}


int main(int argc, char *argv[]) {
    char *host;
    int port, s;
    struct sockaddr_in sock;
    Req *req;
    Res *res;
    char buf[RESSIIZE];
    int success;

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

    req = request(host, port);
    write(s, req, REQSIZE);

    memset(buf, 0, RESSIIZE);
    if (read(s, buf, RESSIIZE) < 1) {
        perror("read error");
        free(req);
        close(s);

        return -1;
    }

    res = (Res *) buf;

    success = (res->cd == 90);
    if (!success) {
        fprintf(stderr, "Unable to traverse the proxy, error code: %d\n", res->cd);

        close(s);
        free(req);

        return -1;
    }

    printf("Successful connection to %s:%d through the proxy...!", host, port);

    close(s);
    free(req);


    return 0;
}