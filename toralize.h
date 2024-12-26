/* toralize.h */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>


#define PROXY     "127.0.0.1"
#define PROXYPORT 9050
#define USERNAME "toraliz"
#define REQSIZE sizeof(struct proxy_request)
#define RESSIIZE sizeof(struct proxy_response)

// type aliases for the packets
typedef unsigned char int8;
typedef unsigned short int int16;
typedef unsigned int int32;

// SOCKS4 CONNECT implementation

/*
                +----+----+----+----+----+----+----+----+----+----+....+----+
                | VN | CD | DSTPORT |      DSTIP        | USERID       |NULL|
                +----+----+----+----+----+----+----+----+----+----+....+----+
    # of bytes:	   1    1      2              4           variable       1
*/

// CONNECT request packet
struct proxy_request {
    int8 vn;
    int8 cd;
    int16 dstport;
    int32 dstip;
    unsigned char userid[8];
};

typedef struct proxy_request Req;

/*
                +----+----+----+----+----+----+----+----+
                | VN | CD | DSTPORT |      DSTIP        |
                +----+----+----+----+----+----+----+----+
    # of bytes:	   1    1      2              4
*/

// CONNECT response packet
struct proxy_response {
    int8 vn;
    int8 cd;
    int16 _;
    int32 __;
};

typedef struct proxy_response Res;

Req *request(const char*, const int);
int main(int, char**);