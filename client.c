#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>       
#include <arpa/inet.h>    

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    char message[BUFFER_SIZE] = {0};
    
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

    while (1) {
        // メッセージ受信
        int valread = read(sock, buffer, BUFFER_SIZE);
        if (valread <= 0) {
            printf("サーバーとの接続が切れました。\n");
            break;
        }

        buffer[valread] = '\0';
        printf("サーバー： %s\n", buffer);

        // メッセージを入力
        printf("返信を入力してください: ");
        fgets(message, BUFFER_SIZE, stdin);
        message[strcspn(message, "\n")] = '\0';

        // 終了コマンド
        if (strcmp(message, "q") == 0) {
            send(sock, message, strlen(message), 0); 
            printf("終了コマンドを送信しました。\n");
            break;
        }
       
        // 返信を送信
        send(sock, message, strlen(message), 0);
        printf("サーバーに返信: %s\n", message);

        // バッファをクリア
        memset(buffer, 0, sizeof(buffer));
        memset(message, 0, sizeof(message));
    }

    // ソケットを閉じる
    close(sock);
    printf("接続を終了しました\n");

    return 0;
}
