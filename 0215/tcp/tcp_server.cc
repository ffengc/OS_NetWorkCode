
#include <memory>
#include "tcp_server.hpp"

// ./tcp_server port
static void Usage(std::string proc)
{
    std::cout << "\nUsage: " << proc << " port\n"
              << std::endl;
}
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(1);
    }
    uint16_t port = atoi(argv[1]);
    std::unique_ptr<TcpServer> svr(new TcpServer(port));
    svr->initServer();
    svr->start();
    return 0;
}