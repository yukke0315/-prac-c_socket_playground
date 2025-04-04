#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>       
#include <arpa/inet.h>    

#define PORT 8080

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char *message = "Hello!";
    
    // ソケットの作成
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket creation error");
        return 1;
    }
    printf("ソケット作成成功！\n");

    // サーバーのアドレスとポートを設定
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("invalid address / address not supported");
        return 1;
    }
    printf("サーバー情報の設定完了！\n");

    // サーバーに接続
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connection failed");
        return 1;
    }
    printf("サーバーに接続成功！\n");

    // メッセージを送信
    send(sock, message, strlen(message), 0);
    printf("メッセージ送信: %s\n", message);

    // ソケットを閉じる
    close(sock);
    printf("接続を終了しました\n");

    return 0;
}
