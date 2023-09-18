#include "Server.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdio>
#include <iostream>

bool Server::_shouldExit = false;

Server::Server(void) : _pollFds(), _socket()
{
    this->mask.sa_handler = Server::gracefulShutdown;
    sigaction(SIGINT, &this->mask, NULL);
    sigaction(SIGTERM, &this->mask, NULL);
    sigaction(SIGTSTP, &this->mask, NULL);
}

Server::Server(short port) : _pollFds(), _socket(port)
{
    this->mask.sa_handler = Server::gracefulShutdown;
    sigaction(SIGINT, &this->mask, NULL);
    sigaction(SIGTERM, &this->mask, NULL);
    sigaction(SIGTSTP, &this->mask, NULL);
}

Server::Server(const Server &other)
{
    *this = other;
}

Server::~Server(void)
{
}

Server &Server::operator=(const Server &other)
{
    if (this != &other)
    {
        this->_pollFds = other._pollFds;
        this->_socket = other._socket;
    }
    return (*this);
}

bool Server::run(void)
{
    std::vector<int> arrayOfFds;
    std::vector<int>::iterator itOfFds;

    if (!this->_socket.bindAndListen())
    {
        std::cerr << "Error: failed to create server TCP socket." << std::endl;
        return (false);
    }
    this->_pollFds.addFd(this->_socket.getFd());
    while (this->_shouldExit != true)
    {
        /* sugestão de melhoria:
		 usar um switch case no lugar do if,
         para ficar mais legível nesse cas
         */
        switch (this->_pollFds.poll())
        {
        case PERROR:
            return (false);
        case PTIMEOUT:
            continue;
        default:
            break;
        }
        if (this->_pollFds.hasNewClientOnQueue())
        {
            this->_pollFds.addFd(this->_socket.acceptClient());
        }
        arrayOfFds = this->_pollFds.getFdsReadyForReading();
        for (itOfFds = arrayOfFds.begin(); itOfFds != arrayOfFds.end(); ++itOfFds)
        {
            this->handleClientData(*itOfFds);
        }
    }
    return (true);
}

void Server::handleClientData(int clientFd)
{
    int bytesRead;
    char buffer[255] = {0};

    bytesRead = ::recv(clientFd, buffer, sizeof(buffer), MSG_DONTWAIT);
    if (bytesRead == -1)
    {
        std::perror("recv");
        this->_pollFds.removeFd(clientFd);
        return;
    }
    if (bytesRead == 0)
    {
        std::cerr << "Client disconnected: " << clientFd << std::endl;
        this->_pollFds.removeFd(clientFd);
        return;
    }
    std::string msg(buffer, buffer + bytesRead);
    std::cerr << "Message from client: " << msg << std::endl;
    ::send(clientFd, "\n", 1, 0);
}

void Server::gracefulShutdown(int signal)
{
    Server::_shouldExit = true;
    std::cerr << "\nReceived signal " << signal << ", shutting down the server." << std::endl;
}
