#include <iostream>
#include <ctime>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <iomanip> // Для std::put_time
#include <sstream> // Для std::ostringstream

#define SERVER_PORT 13 // Порт службы daytime

std::string getCurrentTime() {
    // Получение текущего времени
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);

    // Форматирование времени
    std::ostringstream oss;
    oss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

int main() {
    int sock;
    struct sockaddr_in server_addr, client_addr;
    char buffer[100];
    socklen_t addr_len = sizeof(client_addr);

    // Создание сокета
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cerr << "Ошибка при создании сокета." << std::endl;
        return 1;
    }

    // Настройка адреса сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Привязка сокета к адресу
    if (bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Ошибка при привязке сокета: " << strerror(errno) << std::endl;
        close(sock);
        return 1;
    }

    std::cout << "Сервер daytime запущен на порту " << SERVER_PORT << std::endl;

    while (true) {
        // Получение запроса от клиента
        if (recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &addr_len) < 0) {
            std::cerr << "Ошибка при получении данных: " << strerror(errno) << std::endl;
            continue; // Продолжить цикл в случае ошибки
        }

        // Получение текущего времени в формате "YYYY-MM-DD HH:MM:SS"
        std::string currentTime = getCurrentTime();

        // Вывод времени на терминал
        std::cout << "Отправка времени клиенту: " << currentTime << std::endl;

        // Отправка времени клиенту
        sendto(sock, currentTime.c_str(), currentTime.length(), 0, (struct sockaddr*)&client_addr, addr_len);
    }

    close(sock);
    return 0;
}
