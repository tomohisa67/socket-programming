#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h>     /* for close() */
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define RCVBUFSIZE 32

void DieWithError(char *errorMessage); // エラー時に使う関数
char* get_pickuptime(int c); // 注文時刻から10分後の時刻を計算する. cは待ち人数

void HandleTCPClient(int clntSocket, int c)
{
    char echoBuffer[RCVBUFSIZE];
    int recvMsgSize;
    char str1[] = " does not exist.\n";
    char str2[] = "It's time to pick it up.\n";
    char karaage[] = "karaage\n";
    int str2Len;
    char* echoBuffer2 = get_pickuptime(c); // サーバーからクライアントに送信する文字列 (受け取り時刻)
    char *echoBuffer3;
    int echoBuffer2Len;
    int echoBuffer3Len;

    /* Receive message from client */
    if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
        DieWithError("recv() failed");
    // printf("echoBuffer: %s\n", echoBuffer);

    /* Send received string and receive again until end of transmission */
    while (recvMsgSize > 0) /* zero indicates end of transmission */
    {
        // if (strcmp(echoBuffer, karaage) == 0) // 受信した文字列がkaraageであるかを判定
        if (echoBuffer[0] == karaage[0])
        {
            /* Echo message back to client */
            echoBuffer2Len = strlen(echoBuffer2);

            if (send(clntSocket, echoBuffer2, echoBuffer2Len, 0) != echoBuffer2Len)
                DieWithError("send() failed");
        }
        else // 受信した文字列がkaraageでなかった場合
        { 
            /* Echo message back to client */
            echoBuffer3 = strcat(echoBuffer, str1); // 受信した文字列とstr1を結合
            echoBuffer3Len = strlen(echoBuffer3);

            if (send(clntSocket, echoBuffer3, echoBuffer3Len, 0) != echoBuffer3Len)
                DieWithError("send() failed");
        }

        /* See if there is more data to receive */
        if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
            DieWithError("recv() failed");
    }

    close(clntSocket);    /* Close client socket */
}

char* get_pickuptime(int c) 
{
    time_t timer;     
    struct tm *local;
    int year, month, day, hour, minute, second;
    char* jikoku;

    jikoku = (char*)malloc(sizeof(char) * 4);
     
    timer = time(NULL);  
    timer = timer + 10 * 60 * c; // 待ち人数の分だけ10分追加
    local = localtime(&timer);

    year = local->tm_year + 1900;      
    month = local->tm_mon + 1;        
    day = local->tm_mday;
    hour = local->tm_hour;
    minute = local->tm_min;
    second = local->tm_sec;

    sprintf(jikoku, "%d/%d %d:%d", month, day, hour, minute);
    return jikoku;
 }