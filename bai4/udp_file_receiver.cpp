#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
int checkPort(int* ports,int portsSize,int port){
    for (int i=0;i<portsSize;i++){
        if(ports[i] == port) return i;
    }
    return -1;
}
int main(int argc,char* argv[]){
    char * ipAddr = (char*)malloc(100*sizeof(char));
    strcpy(ipAddr,argv[1]);
    int port;
    sscanf(argv[2],"%d",&port);
    int* ports = (int*)malloc(256*sizeof(int));
    char** files = (char**)malloc(256*sizeof(char*));
    for (int i=0;i<256;i++){
        files[i] = (char*)malloc(256*sizeof(char));
    }
    //size of ports list and files list
    int numOfPort = -1;
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
    // char *filename = (char*)malloc(256*sizeof(char));
    struct sockaddr_in sender_addr;
    socklen_t sender_addr_len = sizeof(sender_addr);

    while(1){
        int ret = recvfrom(receiver,buf,sizeof(buf),0,
        (struct sockaddr*) &sender_addr,&sender_addr_len);
        if(ret < sizeof(buf)){
            buf[ret] = 0;
        }
        port = ntohs(sender_addr.sin_port);
        int portIndex = checkPort(ports,numOfPort+1,port);
        if (portIndex!=-1){
            pt = fopen(files[portIndex],"a");
            fprintf(pt,buf);
            fclose(pt);
        }
        else{
           numOfPort++;
           ports[numOfPort] = port;
           buf[strcspn(buf,"\n")] = '\0';
           strcpy(files[numOfPort],buf);
           strcat(files[numOfPort],".txt");
           char log[256];
           snprintf(log,sizeof(log),"%d %d %s\n",numOfPort,port,files[numOfPort]);
           FILE * ptr = fopen("log.txt","a");
           fprintf(ptr,log);
           fclose(ptr);
        }
        printf("%d bytes received %s:%d: %s\n",ret,
        inet_ntoa(sender_addr.sin_addr),ntohs(sender_addr.sin_port),buf);
    }
    free(ipAddr);
    free(ports);
    for (int i=0;i<256;i++){
        free(files[i]);
    }
    free(files);
}