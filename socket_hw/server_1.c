#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SEND_FILE "test.txt"


//socket的server,client連線
struct sockaddr_in serverInfo,clientInfo;

//inputBuffer 接收資料
//回傳給client端
char inputBuffer[256] = {};
char message[] = {"Good!!! Send success~\n"};

void setting_server(int sockfd)
{
    /*
    serverInfo server資訊
    clientInfo client資訊
    addrlen client資訊長度
    bzero 初始化，將struct(server)涵蓋的bits設為0
    bind() 把自家地址綁到socket上
    listen(sockfd,x) 規定上線人數x
    */
    bzero(&serverInfo,sizeof(serverInfo));
    serverInfo.sin_family = PF_INET;
    serverInfo.sin_addr.s_addr = INADDR_ANY;
    serverInfo.sin_port = htons(4444);
    bind(sockfd,(struct sockaddr *)&serverInfo,sizeof(serverInfo));
    listen(sockfd,5);
}

void recv_msg(int sockfd)
{
    int forClientSockfd = 0;
    int addrlen = sizeof(clientInfo);
    //同意接收clientf請求
    forClientSockfd = accept(sockfd,(struct sockaddr*) &clientInfo, &addrlen);
    //回傳msg
    send(forClientSockfd,message,sizeof(message),0);
    //接收msg
    recv(forClientSockfd,inputBuffer,sizeof(inputBuffer),0);
    printf("Server_recv_msg:\n");
    printf("%s\n",inputBuffer);
}


void send_file(char *filela, int sockfd)
{
    //常數設定
    int forClientSockfd = 0, len = 0;
    int addrlen = sizeof(clientInfo);
    char text[10000],c;
    //同意接收client請求
    forClientSockfd = accept(sockfd,(struct sockaddr*) &clientInfo, &addrlen);
    if (forClientSockfd == -1)
        perror("Server forbidden");


    //開檔案
    FILE *fp = fopen(filela, "r");

    //讀字串至EOF
    while((c = fgetc(fp)) != EOF){
        text[len++] = c;
    }
    //尾端寫下'\0'
    text[len] = '\0';

    printf("[server] Send data to client(%d) \n", forClientSockfd);
    write(forClientSockfd, &len, 4);
    write(forClientSockfd, text, len);
    //關閉client服務
    close(forClientSockfd);
    //關閉檔案
    fclose(fp);
}

int main(int argc , char *argv[])
{
    //socket的建立
    int sockfd = 0;
    sockfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sockfd == -1)
        printf("[X] Server Create Failed\n");
    else
        printf("[V] Server Create Success\n");


    //設定server
    setting_server(sockfd);

    //持續接收請求
    while(1){

        //同意client請求，並傳送file給client
        send_file(SEND_FILE, sockfd);
    }
    return 0;
}