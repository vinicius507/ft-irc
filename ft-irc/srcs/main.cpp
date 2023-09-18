#include "Server.hpp"
#include <iostream>
#include <sstream>

int main(int argc, char **argv)
{
    if (argc == 2)
    {
        size_t port;
        std::stringstream ss(argv[1]);

        if (!(ss >> port) || port < 1024 || port > 65535)
        {
            std::cerr << "Error: invalid port number." << std::endl;
            return (1);
        }
        Server server(port);
        if (!server.run())
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
