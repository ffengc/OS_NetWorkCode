

#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>

#define RECV_SIZE 1024

void usage(std::string proc)
{
    std::cout << "\nUsage: " << proc << "serverIp serverPort\n"
              << std::endl;
}

// ./tcp_client targetIp targetPort
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        usage(argv[0]);
        exit(1);
    }
    //
    std::string server_ip = argv[1];
    uint16_t server_port = atoi(argv[2]);
    // create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        std::cerr << "socket error" << std::endl;
        exit(2);
    }
    // client 不用 bind
    // 不需要显式bind，但是一定需要port
    // 让os自己选择port
    // 客户端需要监听需要accept吗？不需要，客户端必须具有连接别人的能力
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(server_port);
    server.sin_addr.s_addr = inet_addr(server_ip.c_str());
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        std::cerr << "connect error" << std::endl;
        exit(3);
    }
    std::cout << "connect success" << std::endl;
    while (true)
    {
        std::string line;
        std::cout << "请输入# ";
        std::getline(std::cin, line);
        send(sock, line.c_str(), line.size(), 0); // send 是针对tcp的, sendto 是针对udp的
        char buffer[RECV_SIZE];
        ssize_t s = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (s > 0)
        {
            buffer[s] = 0;
            std::cout << "server echo # " << buffer << std::endl;
        }
        else if (s == 0)
        {
            break;
        }
        else
            break;
    }
    close(sock);
    return 0;
}