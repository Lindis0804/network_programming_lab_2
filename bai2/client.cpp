#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
using namespace std;
int main(int argc,char* argv[]){
    char * ipAddr = new char[20];
    strcpy(ipAddr,argv[1]);
    int port;
    sscanf(argv[2],"%d",&port);
    //initialize socket
    int client = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if (client == -1){
        printf("socket() failed.");
        return 1;
    }
    //khai báo cấu trúc địa chỉ client
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ipAddr);
    addr.sin_port = htons(port);
    //kết nối đến server
    int ret = connect(client,(struct sockaddr *)&addr,sizeof(addr));
    if (ret == -1){
        perror("connect() failed.");
        return 1;
    }
    char buf[256];
    //nhận dữ liệu từ server
    ret = recv(client,buf,sizeof(buf),0);
    if(ret <=0){
        printf("Connection closed\n");
        return 1;
    }
    if (ret < sizeof(buf)){
        buf[ret] = 0;
    }
    printf("%d bytes received: %s\n",ret,buf);
    

    //gửi dữ liệu đến server
    FILE *pt;
    pt = fopen("geats.txt","r");
    while (fgets(buf,sizeof(buf),pt)){
        printf("%s\n",buf);
        send(client,buf,strlen(buf),0);
    }
    // while (1){
    //     printf("Enter string: ");
    //     fgets(buf,sizeof(buf),stdin);
    //     send(client,buf,strlen(buf),0);
    //     if (strncmp(buf,"exit",4) == 0)
    //     break;
    // }
    close(client);
    delete[] ipAddr;
    return 0;

}