#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MyClient.h"

// #define BUFSIZE 256

// class Buffer
// {
// public:
//     Buffer();   // コンストラクタ
//     ~Buffer();  // デストラクタ
//     Buffer operator=(unsigned short val);
//     Buffer operator=(unsigned char);
// private:
//     unsigned char buffer[BUFSIZE];  // バッファ
//     int pos;    // バッファ位置
// };

// コンストラクタ
MyClient::MyClient(const char *deststr) : isConnected(false), tranzactionid(0)
{
    // WSA初期化
    err = WSAStartup(MAKEWORD(2, 0), &wsa);

    // 構造体初期化
    memset(&server, 0, sizeof(server));

    // ソケット取得
    sock = socket(AF_INET, SOCK_STREAM, 0);

    // サーバー設定
    server.sin_family = AF_INET;
    server.sin_port = htons(MODBUS_PORT);
    server.sin_addr.S_un.S_addr = inet_addr(deststr);
}

// デストラクタ
MyClient::~MyClient()
{
    // 接続済みフラグを解除
    isConnected = false;
    // ソケットクローズ
    closesocket(sock);
    // WSA終了
    WSACleanup();
}

// エラーコード取得
int MyClient::error() const
{
    return err;
}

// サーバーに接続
bool MyClient::connectToServer()
{
    if(connect(sock, (struct sockaddr *)&server, sizeof(server)) != 0)
    {
        // 通信が失敗した場合
        return false;
    }
    return true;
}

// 通信開始
void MyClient::start()
{
    int retryCnt = 0;

    // コネクト
    if(!isConnected)
    {
        while(true)
        {
            if(connectToServer())
            {
                printf("connected\n");
                isConnected = true;
                retryCnt = 0;
                break;
            }
            else
            {
                DWORD errcode = GetLastError();
                printf("connect failed: %d\n", errcode);
                // 3回以上リトライしたら終了
                if(retryCnt > 3)
                {
                    retryCnt = 0;
                    return;
                }
            }
            retryCnt++;
        }
    }

    // 読み込み
    while(true)
    {
        readMemory();
    }
}

bool MyClient::readMemory()
{
    char recvbuf[256] = {0};
    char sendbuf[256] = {0};
    int sendpos = 0;
    int recvpos = 0;
    unsigned short readdata;

    access.addr = 100;
    access.cnt = 1;

    // トランザクション番号
    sendbuf[sendpos] = *((const char *)(&tranzactionid) + 1);
    sendpos++;
    sendbuf[sendpos] = *((const char *)(&tranzactionid));
    sendpos++;
    // プロトコル識別子
    sendbuf[sendpos] = 0;
    sendpos += 2;
    // データ長
    sendbuf[sendpos] = 0;
    sendpos++;
    sendbuf[sendpos] = D_FUNC_READ_LENGTH;
    sendpos++;
    // ユニット識別子
    sendbuf[sendpos] = UNIT_ID;
    sendpos++;
    // ファンクションコード
    sendbuf[sendpos] = D_FUNC_READ;
    sendpos++;
    // 開始アドレス
    sendbuf[sendpos] = *((const char *)(&access.addr) + 1);
    sendpos++;
    sendbuf[sendpos] = *((const char *)(&access.addr));
    sendpos++;
    // レジスタ数
    sendbuf[sendpos] = *((const char *)(&access.cnt) + 1);
    sendpos++;
    sendbuf[sendpos] = *((const char *)(&access.cnt));
    sendpos++;

    send(sock, sendbuf, sendpos, 0);

    recv(sock, recvbuf, 11, 0);

    *((char *)(&readdata) + 1) = recvbuf[9];
    *((char *)(&readdata)) = recvbuf[10];

    printf("recv: %d\n", readdata);

    return true;
}
