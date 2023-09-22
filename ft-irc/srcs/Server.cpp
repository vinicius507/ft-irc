#include "Server.hpp"
#include "Message.hpp"
#include <algorithm>
#include <arpa/inet.h>
#include <cstdio>
#include <iostream>
#include <netinet/in.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>

bool Server::_shouldExit = false;

Server::Server(void) : _pollFds(), _socket() {}
Server::Server(short port) : _pollFds(), _socket(port) {}

Server::Server(const Server &other)
    : _pollFds(other._pollFds), _socket(other._socket) {}

Server::~Server(void) {}

Server &Server::operator=(const Server &other) {
  if (this != &other) {
    this->_pollFds = other._pollFds;
    this->_socket = other._socket;
  }
  return (*this);
}

bool Server::run(void) {
  int clientFd;
  std::vector<int> arrayOfFds;
  std::vector<int>::iterator itOfFds;

  if (!this->_socket.bindAndListen()) {
    std::cerr << "Error: failed to create server TCP socket." << std::endl;
    return (false);
  }
  this->_pollFds.addFd(this->_socket.getFd());
  while (this->_shouldExit != true) {
    switch (this->_pollFds.poll()) {
    case PERROR:
      return (false);
    case PTIMEOUT:
      continue;
    default:
      break;
    }
    if (this->_pollFds.hasNewClientOnQueue()) {
      clientFd = this->_socket.acceptClient();
      if (clientFd != -1) {
        this->_clients.addClient(clientFd);
        this->_pollFds.addFd(clientFd);
      } else {
        std::cerr << "Error: couldn't accept new client" << std::endl;
      }
    }
    arrayOfFds = this->_pollFds.getFdsReadyForReading();
    for (itOfFds = arrayOfFds.begin(); itOfFds != arrayOfFds.end(); ++itOfFds) {
      if (!this->handleClientData(this->_clients[*itOfFds])) {
        this->_clients.disconnectClient(clientFd);
        this->_pollFds.removeFd(clientFd);
      }
    }
  }
  return (true);
}

bool Server::handleClientData(Client &client) {
  Message msg;
  int bytesRead;
  char buf[255] = {0};
  std::string::size_type crlf;

  bytesRead = ::recv(client.getFd(), buf, sizeof(buf), MSG_DONTWAIT);
  if (bytesRead == -1) {
    std::perror("recv");
    return (false);
  }
  if (bytesRead == 0) {
    std::cerr << "Client disconnected: " << client.getFd() << std::endl;
    return (false);
  }
  std::string &clientBuf = client.getBuffer();
  clientBuf += std::string(buf, buf + bytesRead);
  crlf = clientBuf.find("\r\n");
  if (crlf != std::string::npos) {
    msg = parseIrcMessage(clientBuf.substr(0, crlf - 2));
    send(client.getFd(), clientBuf.substr(0, crlf - 2).data(), crlf - 2, 0);
  }
  return (true);
}

void Server::gracefulShutdown(int signal) {
  Server::_shouldExit = true;
  std::cerr << "\nReceived signal " << signal << ", shutting down the server."
            << std::endl;
}
