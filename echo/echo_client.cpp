#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 7 // Порт службы echo
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Создание сокета
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Ошибка при создании сокета." << std::endl;
        return 1;
    }

    // Настройка адреса сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr("172.16.40.13"); // IP-адрес сервера

    // Подключение к серверу
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Ошибка при подключении к серверу." << std::endl;
        close(sock);
        return 1;
    }

    std::cout << "Введите сообщение для отправки на сервер: ";
    std::cin.getline(buffer, BUFFER_SIZE);

    // Вывод введенного сообщения в консоль
    std::cout << "Вы ввели сообщение: " << buffer << std::endl;

    // Отправка сообщения на сервер
    send(sock, buffer, strlen(buffer), 0);

    // Получение ответа от сервера
    ssize_t len = recv(sock, buffer, BUFFER_SIZE - 1, 0);
    
    if (len < 0) {
        std::cerr << "Ошибка при получении данных." << std::endl;
        close(sock);
        return 1;
    }

    buffer[len] = '0'; // Завершаем строку правильно
    std::cout << "Ответ от сервера: " << buffer << std::endl;

    // Закрытие сокета
    close(sock);
    return 0;
}
