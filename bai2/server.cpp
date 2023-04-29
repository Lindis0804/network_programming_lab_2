#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <bits/stdc++.h>
using namespace std;
int* countString(char* str,char *sub,int strSize,int subSize){
    int s = 0;
    int i =0;
    int start = 0;
    int* res =(int*) malloc(2*sizeof(int));
    res[0] = 0;
    res[1] = strSize;
    while (i<strSize){
        if (str[i] == sub[0]){
            int j = 1;
            int count = 0;
            start = 0;
            for (j=1;j<10;j++){
                if (i+j>=strSize){
                     res[1] = i;
                     i=i+j;
                     break;
                }
                else{
                   if (str[i+j] == sub[0]){
                    if (count == 0) start = i+j;
                    count++;
                   }
                   if (str[i+j]!=sub[j]) break;
                }
                //printf("i+j: %d,j: %d,start: %d\n",i+j,j,start);
            }
            if (j>subSize-1) {
                i = i+subSize;
                s++;
            }
            else{
                i = start != 0?start:i+1;
            }
        }
        else{
            i++;
        }
    }
    res[0]= s;
    return res;

}
char* subStr(char* str,int size,int start,int end){
    char * res =(char*) malloc(256*sizeof(char));
    strcpy(res,"");
     if (end == -1){
        end = size-1;
     }
        for (int i=0;i<end-start+1;i++){
            res[i] = str[i+start];
        }
        return res;
}
int main(int argc,char* argv[]){
    char * ipAddr = new char[20];
    int port;
    printf("%s %s\n",argv[1],argv[2]);
    strcpy(ipAddr,argv[1]);
    sscanf(argv[2],"%d",&port);
    //initialize socket
    int listener  = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if (listener == -1){
        printf("socket() failed.");
        return 1;
    }
    /*
    khai báo cấu trúc địa chỉ server
    */
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ipAddr);
    addr.sin_port = htons(port);

    // bind cấu trúc địa chỉ server với socket
    if (bind(listener,(struct sockaddr *)&addr,sizeof(addr))){
        printf("ipAddr: %s port: %d\n",ipAddr,port);
        printf("bind() failed.\n");
        return 1;
    }
    printf("Waiting for new client...\n");
    //lắng nghe kết nối
    if (listen(listener,5)){
        printf("listen() failed.");
        return 1;
    }
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    
    // chấp nhận kết nối
    int client = accept(listener,(struct sockaddr*)&clientAddr,&clientAddrLen);
    printf("Client IP: %s:%d\n",inet_ntoa(clientAddr.sin_addr),ntohs(clientAddr.sin_port));

    // gửi đến client
    char * msg = "Hello client\n";
    send(client,msg,strlen(msg),0);

    // nhận từ client
    char buf[256];
    int count = 0;
    char* sub = "0123456789";
    int* countStr = (int*) malloc(2*sizeof(int));
    char* tail = (char*) malloc(256*sizeof(char));
    strcpy(tail,"");
    while(1){
        int ret = recv(client,buf,sizeof(buf),0);
        if (ret <= 0){
            printf("Ket noi bi dong\n");
            break;
        }
        if (ret < sizeof(buf)){
            buf[ret] = 0;
        }
        //chuyển dấu cách cuối thành dấu '\0'
        buf[strcspn(buf,"\n")] = '\0';
        printf("%d bytes received\n",ret);
        printf("%s\n",buf);
        //nối đuôi tách ra ở chuỗi trước vào lời nhắn hiện tại
        strcat(tail,buf);
        //printf("tail: %s \n",tail);

        //xử lý đếm số sub
        countStr = countString(tail,sub,strlen(tail),strlen(sub));
        count += countStr[0];
        //printf("pos: %d\n",countStr[1]);
        tail = subStr(tail,strlen(tail),countStr[1],-1);
        //printf("tail: %s \n",tail);
    
    }
    printf("Number of sub: %d\n",count);
    close(client);
    close(listener);
    delete[] ipAddr;
    free(countStr);
    free(tail);
    return 0;
}