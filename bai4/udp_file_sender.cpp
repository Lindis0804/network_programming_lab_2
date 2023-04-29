#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
int main(int argc,char * argv[]){
    char * ipAddr = (char*)malloc(100*sizeof(char));
    strcpy(ipAddr,argv[1]);
    int port;
    sscanf(argv[2],"%d",&port);
    char *filename = (char*)malloc(256*sizeof(char));
    strcpy(filename,argv[3]);
    char *sourcefile = (char*)malloc(256*sizeof(char));
    strcpy(sourcefile,argv[4]);
    //khởi tại socket
    int sender = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ipAddr);
    addr.sin_port = htons(port);
    char buf[256];
    //truyền tên filename
    int ret = sendto(sender,filename,sizeof(filename),0,
    (struct sockaddr*)&addr,sizeof(addr)
    );
    printf("%d bytes of filename sent.\n",ret);
    FILE * pt;
    pt = fopen(sourcefile,"r");
    while (fgets(buf,sizeof(buf),pt)){
        printf("Buf: %s\n",buf);
        int ret = sendto(sender,buf,strlen(buf),0,
        (struct sockaddr*)&addr, sizeof(addr)
        );
        printf("%d bytes sent.\n",ret);
        usleep(1000000);
    }
    return 0;
}