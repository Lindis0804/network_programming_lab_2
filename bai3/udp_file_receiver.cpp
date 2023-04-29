#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
int main(int argc,char* argv[]){
    char * ipAddr = (char*)malloc(100*sizeof(char));
    strcpy(ipAddr,argv[1]);
    int port;
    sscanf(argv[2],"%d",&port);
    //initialize socket
    int receiver = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);

    // khai báo cấu trúc địa chỉ server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ipAddr);
    addr.sin_port = htons(port);

    //bind
    bind(receiver,(struct sockaddr*) &addr,sizeof(addr));
    
    FILE* pt;
    char buf[256];
    char *filename = (char*)malloc(256*sizeof(char));
    struct sockaddr_in sender_addr;
    socklen_t sender_addr_len = sizeof(sender_addr);

    //lấy tên file
    int ret = recvfrom(receiver,filename,sizeof(filename),0,
    (struct sockaddr*) &sender_addr,&sender_addr_len
    );
    if (ret<sizeof(filename)){
        filename[ret] = 0;
    }
    strcat(filename,".txt");
    printf("filename:%s\n",filename);
    while(1){
        int ret = recvfrom(receiver,buf,sizeof(buf),0,
        (struct sockaddr*) &sender_addr,&sender_addr_len);
        if(ret < sizeof(buf)){
            buf[ret] = 0;
        }
        printf("%d bytes received %s: %s\n",ret,
        inet_ntoa(sender_addr.sin_addr),buf);

        pt = fopen(filename,"a");
        fprintf(pt,buf);
        fclose(pt);
    }
}