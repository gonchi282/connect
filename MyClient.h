#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <winsock2.h>

#define MODBUS_PORT 502         // ModbusTCPポート
#define PRTCL_ID    0x00        // プロトコル識別子
#define UNIT_ID     0xff        // ユニット識別子
#define D_FUNC_READ 0x03        // Read Holding Register
#define D_FUNC_READ_LENGTH 0x06 // Read Holding Register 読み込みデータ長

class MyClient
{
public:
    MyClient(const char *deststr);      // コンストラクタ
    ~MyClient();                        // デストラクタ
    int error() const;                  // エラーコード取得
    bool connectToServer();             // 通信
    void start();                       // 通信開始
    bool readMemory();                  // 読み込み
private:
    WSADATA wsa;                        // WSAデータ
    SOCKET sock;                        // socket
    int err;                            // エラーコード
    struct sockaddr_in server;          // サーバーのソケットアドレス構造体
    bool isConnected;                   // 接続済み
    struct access                       // アクセス構造体
    {
        unsigned char funccode;
        unsigned short addr;
        unsigned short cnt;
    }access;
    unsigned short tranzactionid;       // トランザクションID
};

#endif // MYCLIENT_H