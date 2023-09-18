#include "Server.hpp"
#include <csignal>
#include <iostream>
#include <sstream>

inline void configureSignalHandlers(void)
{
    struct sigaction mask;
    mask.sa_handler = Server::gracefulShutdown;

    sigaction(SIGINT, &mask, NULL);
    sigaction(SIGTERM, &mask, NULL);
    sigaction(SIGTSTP, &mask, NULL);
}

int main(int argc, char **argv)
{
    if (argc == 2)
    {
        size_t port;
        std::stringstream ss(argv[1]);

        if (!(ss >> port) || ss.fail() || !ss.eof())
        {
            std::cerr << "Error: invalid argument." << std::endl;
            return (1);
        }
        if (port < 1024 || port > 65535)
        {
            std::cerr << "Error: invalid port number." << std::endl;
            return (1);
        }
        configureSignalHandlers();
        Server server(port);
        if (server.run() == false)
        {
            std::cerr << "Error: failed to run server." << std::endl;
            return (1);
        }
    }
    else
    {
        std::cerr << "Usage: " << argv[0] << " PORT" << std::endl;
        return (1);
    }
    return (0);
}
