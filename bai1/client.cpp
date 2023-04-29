#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
using namespace std;
void clean_stdin(void){
    int c;
    do{
        c = getchar();
    }while (c!='\n' && c!=EOF);
}
int main(int argc,char* argv[]){
    char * ipAddr = new char[20];
    char delimiter[5] = "\r\n";
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
    char lap_name[256];
    char disk;
    int disk_size;
    char disk_info[256];
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
    while (1){
        strcpy(buf,"");
        printf("Enter laptop's name or exit to escape: ");
        fgets(lap_name,sizeof(lap_name),stdin);
        lap_name[strcspn(lap_name,"\n")] = '\0';
        if (strncmp(buf,"exit",4) == 0)
        break;
        strcat(buf,lap_name);
        strcat(buf,delimiter);
        while (1){
          printf("Input disk or 0 to exit: ");
          scanf("%c",&disk);
          if (disk == '0'){
            break;
          }
          printf("Input disk size or -1 to exit: ");
          scanf("%d",&disk_size);
          if (disk_size == -1){
            break;
          }
          snprintf(disk_info,sizeof(disk_info),"%c - %dGB",disk,disk_size);
          strcat(buf,disk_info);
          strcat(buf,delimiter);
          clean_stdin();
        }
        printf("%d %s",strlen(buf),buf);
        send(client,buf,strlen(buf),0);
        clean_stdin();
    }
    close(client);
    delete[] ipAddr;
    return 0;

}