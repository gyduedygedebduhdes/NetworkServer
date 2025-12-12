#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <winsock2.h>

#define CLIENTNUM 2

#define BUFFER 256

#define RECVOK 1
#define RECVNO -1

#pragma comment(lib, "ws2_32.lib")

unsigned __stdcall MultiThreads(void* pArguments);


int main()
{

    HANDLE hThread[CLIENTNUM] = { NULL,NULL };
    unsigned threadID[CLIENTNUM] = { 0,0 };


    for (int i = 0; i < CLIENTNUM; ++i)
    {
        hThread[i] = (HANDLE)_beginthreadex(NULL, 0, &MultiThreads, (void*)i, 0, &threadID[i]);
        Sleep(1);

    }


    for (int i = 0; i < CLIENTNUM; ++i)
    {
        WaitForSingleObject(hThread[i], INFINITE);
    }

    for (int i = 0; i < CLIENTNUM; ++i)
    {
        CloseHandle(hThread[i]);
    }




    return 0;
}

unsigned __stdcall MultiThreads(void* pArguments)
{
    printf("ThreadNo.%d\n", (int)pArguments);

    char (*buffer2)[BUFFER];  // 「要素256のchar配列」へのポインタ

    buffer2 = malloc(sizeof(char[CLIENTNUM][BUFFER]));   // 2 × 256 を確保


    const char* message = "Hello, Server!";

    int bytesSent[CLIENTNUM];

    WSADATA wsaData[CLIENTNUM];
    SOCKET serverSocket[CLIENTNUM];

    for (int i = 0; i < CLIENTNUM; ++i)
    {
        WSAStartup(MAKEWORD(2, 2), &wsaData[i]);

    }



    for (int i = 0; i < CLIENTNUM; ++i)
    {
        serverSocket[i] = socket(AF_INET, SOCK_STREAM, 0);
    }



    struct sockaddr_in serverAddr[CLIENTNUM];




    serverAddr[0].sin_family = AF_INET;
    serverAddr[0].sin_addr.s_addr = INADDR_ANY; // 自分のIP
    serverAddr[0].sin_port = htons(5000);       // ポート番号

    serverAddr[1].sin_family = AF_INET;
    serverAddr[1].sin_addr.s_addr = INADDR_ANY; // 自分のIP
    serverAddr[1].sin_port = htons(6000);       // ポート番号

    for (int i = 0; i < CLIENTNUM; ++i)
    {
        bind(serverSocket[i], (struct sockaddr*)&serverAddr[i], sizeof(serverAddr[i]));
    }

    for (int i = 0; i < CLIENTNUM; ++i)
    {
        listen(serverSocket[i], SOMAXCONN);
    }


    printf("サーバー起動中... クライアント接続待ち\n");

    struct sockaddr_in clientAddr[CLIENTNUM];
    int clientSize[CLIENTNUM];
    for (int i = 0; i < CLIENTNUM; ++i)
    {
        clientSize[i] = sizeof(clientAddr[i]);

    }

    SOCKET clientSocket[CLIENTNUM];

    while (1)
    {



        if ((clientSocket[0] = accept(serverSocket[0], (struct sockaddr*)&clientAddr[0], &clientSize[0])) != NULL)
        {
            break;
        }
        
        if ((clientSocket[1] = accept(serverSocket[1], (struct sockaddr*)&clientAddr[1], &clientSize[1])) != NULL)
        {
            break;
        }


    }






    int bytesReceived[CLIENTNUM];



    while (1)
    {

        if ((bytesReceived[0] = recv(clientSocket[0], buffer2[0], sizeof(buffer2[0]), 0)) != SOCKET_ERROR)
        {
            break;
        }
        if ((bytesReceived[1] = recv(clientSocket[1], buffer2[1], sizeof(buffer2[1]), 0)) != SOCKET_ERROR)
        {
            break;
        }
    }





    while (1)
    {
        // データの送信


        if ((bytesSent[0] = send(clientSocket[0], message, strlen(message), 0)) != SOCKET_ERROR)
        {
            printf("Send1 success\n");
            break;


        }

        if ((bytesSent[1] = send(clientSocket[1], message, strlen(message), 0)) != SOCKET_ERROR)
        {
            printf("Send1 success\n");
            break;


        }
    }



    for (int i = 0; i < CLIENTNUM; i++)
    {
        closesocket(clientSocket[i]);
        closesocket(serverSocket[i]);

    }
    WSACleanup();
    free(buffer2);

    return 0;

}