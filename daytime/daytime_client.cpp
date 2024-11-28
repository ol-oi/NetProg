#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 13
#define SERVER_IP "127.0.0.1" // Локальный адрес сервера

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[100];

    // Создание сокета
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cerr << "Ошибка при создании сокета." << std::endl;
        return 1;
    }

    // Настройка адреса сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // Отправка запроса серверу
    sendto(sock, "TIME", strlen("TIME"), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // Получение ответа от сервера
    socklen_t addr_len = sizeof(server_addr);
    ssize_t n = recvfrom(sock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&server_addr, &addr_len);
    if (n < 0) {
        std::cerr << "Ошибка при получении данных." << std::endl;
    } else {
        buffer[n] = '0'; // Завершение строки
        std::cout << "Полученное время: " << buffer << std::endl;
    }

    close(sock);
    return 0;
}
