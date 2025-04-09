#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>       
#include <arpa/inet.h> 
#include <sys/select.h>   

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

    fd_set read_fds;
    int max_fd = sock > STDIN_FILENO ? sock : STDIN_FILENO;

    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);
        FD_SET(sock, &read_fds);

        int activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        if (activity < 0) {
            perror("select error");
            break;
        }

        // 自分からの入力
        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
            // メッセージを入力
            printf("メッセージを入力してください: ");
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
        }

        // サーバーからのメッセージ
        if (FD_ISSET(sock, &read_fds)){
            // メッセージ受信
            int valread = read(sock, buffer, BUFFER_SIZE);
            if (valread <= 0) {
                printf("サーバーとの接続が切れました。\n");
                break;
            }

            buffer[valread] = '\0';
            printf("サーバー： %s\n", buffer);
        }

        // // メッセージを入力
        // printf("返信を入力してください: ");
        // fgets(message, BUFFER_SIZE, stdin);
        // message[strcspn(message, "\n")] = '\0';

        // // 終了コマンド
        // if (strcmp(message, "q") == 0) {
        //     send(sock, message, strlen(message), 0); 
        //     printf("終了コマンドを送信しました。\n");
        //     break;
        // }
       
        // // 返信を送信
        // send(sock, message, strlen(message), 0);
        // printf("サーバーに返信: %s\n", message);

        // バッファをクリア
        memset(buffer, 0, sizeof(buffer));
        memset(message, 0, sizeof(message));
    }

    // ソケットを閉じる
    close(sock);
    printf("接続を終了しました\n");

    return 0;
}
