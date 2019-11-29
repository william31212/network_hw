#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define TARGET_IP "192.168.43.18"
#define TARGET_PORT 4444
#define SEND_MSG "whatthebbbb"
#define FILE_NAME "get2.txt"


void connect_to_server(char *ip, int port, int sockfd)
{
    struct sockaddr_in info;

    //初始化，將struct涵蓋的bits設為0
    bzero(&info,sizeof(info));
    //BSD是AF, POSIX是PF
    //sockaddr_in為Ipv4結構
    info.sin_family = PF_INET;
    //IP address setting
    info.sin_addr.s_addr = inet_addr(ip);
    //htons -> Host TO Network Short integer
    //將port(int)轉換成port(network_short_int)
    info.sin_port = htons(port);

    int err = connect(sockfd,(struct sockaddr *)&info,sizeof(info));
    if(err == -1)
        printf("[X] Connection Falied\n");
    else
        printf("[V] Connection Success\n");
}


void send_msg_to_server(char *msg, int sockfd)
{
	//接收資訊buffer
    char recvmsg[100] = {};
    //send,recv
    /*
    sd: socketfd
    buf: buffer塞收到的資料
    len: buffer大小
    flags: 預設為0
    ret val:
        - val=0
            - 收到的資料總Byte
        - val!=0
            - 真的0Byte
            - domain設定不一致
            - 一方正常情況結束連線
    */
    send(sockfd, msg, strlen(msg), 0);
    recv(sockfd, recvmsg, sizeof(recvmsg), 0);
    printf("%s", recvmsg);
    printf("---Close Socket---\n");
    close(sockfd);
    return;
}

void download(char *filename, int sockfd)
{
    int len=0;
    char text[500];
    printf("[client] Read data from server\n");
    read(sockfd, &len, 4);
    read(sockfd, text, len);

    //開檔寫入從server來的file
    FILE *fp = fopen(filename, "w");
    //將text裡的字串傳入
    fprintf(fp, "%s", text);
    //關閉client服務
    close(sockfd);
    //關閉file
    fclose(fp);
}



int main(int argc, char *argv[])
{
    //AF_INET 使用的是IPv4協定
    //SOCK_STREAM 序列化的連接導向位元流，可以做位元流傳輸
    //讓kernel選擇type對應的默認協議
    //socket創建失敗則會回傳-1
    int sockfd = 0;
    sockfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sockfd == -1)
        printf("[X] Create Failed\n");
    else
    	printf("[V] Create Success\n");


    //socket的連線
    connect_to_server(TARGET_IP, TARGET_PORT, sockfd);
    // 送msg到server上
    // send_msg_to_server(SEND_MSG, sockfd);
    // 從server載檔案
    download(FILE_NAME, sockfd);

    return 0;
}