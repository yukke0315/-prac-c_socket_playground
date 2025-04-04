#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    char *response = "Hello, Client!";

    // ソケットの作成
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("socket failed");
        return 1;
    }
    printf("サーバーソケット作成成功！\n");

    // ソケットにアドレスとポートを割り当て
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        return 1;
    }
    printf("ポート %d でバインド完了\n", PORT);

    // 接続待ち状態にする
    if (listen(server_fd, 5) < 0) {
        perror("listen failed");
        return 1;
    }
    printf("接続待ち中...\n");

    // クライアントの接続を受け付ける
    client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (client_fd < 0) {
        perror("accept failed");
        return 1;
    }
    printf("クライアント接続受け付け\n");

    // メッセージ受信
    int valread = read(client_fd, buffer, BUFFER_SIZE);
    printf("クライアントから受信: %s\n", buffer);

    // 返信を送信
    send(client_fd, response, strlen(response), 0);
    printf("クライアントに返信: %s\n", response);

    // ソケットを閉じる
    close(client_fd);
    close(server_fd);
    printf("接続を終了しました\n");

    return 0;
}
