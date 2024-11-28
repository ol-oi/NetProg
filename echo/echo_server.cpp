#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 7  // Порт для echo-сервера

int main() {
    int sockfd, newsockfd;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    char buffer[256];
    
    // Создание сокета
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Ошибка при открытии сокета" << std::endl;
        return 1;
    }

    // Настройка адреса сервера
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    // Привязка сокета к порту
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Ошибка при привязке" << std::endl;
        return 1;
    }

    // Прослушивание входящих соединений
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    std::cout << "Сервер запущен. Ожидание подключений..." << std::endl;

    // Цикл для обработки подключений
    while (true) {
        // Принятие входящего соединения
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) {
            std::cerr << "Ошибка при принятии соединения" << std::endl;
            continue;
        }

        // Чтение данных из сокета
        memset(buffer, 0, sizeof(buffer)); // Обнуление буфера
        int n = read(newsockfd, buffer, sizeof(buffer) - 1); // Читаем данные
        if (n < 0) {
            std::cerr << "Ошибка при чтении из сокета" << std::endl;
            close(newsockfd);
            continue;
        }

        // Запись полученного сообщения в файл
        std::ofstream outfile("received_messages.txt", std::ios::app);
        if (outfile.is_open()) {
            outfile << buffer; // Записываем в файл
            outfile.close();
        } else {
            std::cerr << "Ошибка при открытии файла" << std::endl;
        }

        // Отправка ответа клиенту (без лишних символов)
        n = write(newsockfd, buffer, n); // Отправляем обратно столько байт, сколько прочитали
        if (n < 0) {
            std::cerr << "Ошибка при записи в сокет" << std::endl;
        }

        close(newsockfd); // Закрываем соединение с клиентом
    }

    close(sockfd); // Закрываем основной сокет
    return 0;
}
