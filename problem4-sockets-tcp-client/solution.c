#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Программе передаются два аргумента:
//   argv[1] — IPv4-адрес сервера в десятичной записи (например, "127.0.0.1")
//   argv[2] — номер порта
//
// Программа должна:
//   1. Установить TCP-соединение с указанным сервером.
//   2. В цикле читать со stdin целые знаковые числа в текстовом формате.
//   3. Отправлять каждое число на сервер в бинарном виде (int32, Little Endian).
//   4. Получать от сервера int32 LE в ответ и выводить его в stdout в текстовом виде.
//   5. Если сервер закрыл соединение — завершиться с кодом возврата 0.

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <ipv4_addr> <port>\n", argv[0]);
        return 1;
    }

    // TODO: создайте TCP-сокет (AF_INET, SOCK_STREAM),
    //       заполните struct sockaddr_in с помощью inet_aton/inet_pton,
    //       подключитесь через connect,
    //       реализуйте цикл чтения/отправки/приёма/вывода чисел.
    //       Порядок байт — Little Endian (на x86/x86_64 это нативный порядок).
    struct in_addr ip_addr;
    if (!inet_aton(argv[1], &ip_addr)) {
        fprintf(stderr, "Invalid IP address\n");
        return 1;
    }
    int port = atoi(argv[2]);
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        return 1;
    }
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr = ip_addr;

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        close(sock);
        return 1;
    }

    int32_t number;
    while (scanf("%d", (int*)&number) == 1) {
        int32_t le_number = htole32(number);
        ssize_t sent = send(sock, &le_number, sizeof(le_number), 0);
        if (sent != sizeof(le_number)) {
            perror("send");
            break;
        }

        int32_t response_le;
        ssize_t received = recv(sock, &response_le, sizeof(response_le), MSG_WAITALL);
        if (received == 0) {
            break;
        }
        if (received != sizeof(response_le)) {
            perror("recv");
            break;
        }
        int32_t response = le32toh(response_le);
        printf("%d\n", response);
    }

    close(sock);
    return 0;
    return 0;
}
