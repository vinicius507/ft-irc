#include "ServerSocket.hpp"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

ServerSocket::ServerSocket(void) : _socketFd(-1), _port(6667) {}

ServerSocket::ServerSocket(short port) : _socketFd(-1), _port(port) {}

ServerSocket::ServerSocket(const ServerSocket &other) : _socketFd(other._socketFd), _port(other._port) {}

ServerSocket::~ServerSocket(void) { this->closeSocket(); }

ServerSocket &ServerSocket::operator=(const ServerSocket &other) {
  if (this != &other) {
    this->closeSocket();
    this->_port = other._port;
    this->_socketFd = other._socketFd;
  }
  return (*this);
}

bool ServerSocket::bindAndListen(void) {
  sockaddr_in serverAddr;

  this->_socketFd = socket(AF_INET, SOCK_STREAM, 0);
  if (this->_socketFd == -1) {
    std::perror("socket");
    return (false);
  }
  std::memset(&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = INADDR_ANY;
  serverAddr.sin_port = htons(this->_port);
  int bindResult = bind(this->_socketFd, reinterpret_cast<struct sockaddr *>(&serverAddr), sizeof(serverAddr));
  if (bindResult == -1) {
    std::perror("bind");
    this->closeSocket();
    return (false);
  }
  if (listen(this->_socketFd, 10) == -1) {
    std::perror("listen");
    this->closeSocket();
    return (false);
  }
  std::cerr << "Started listening on 0.0.0.0:" << this->_port << "." << std::endl;
  return (true);
}

int ServerSocket::getFd(void) const { return (this->_socketFd); }

int ServerSocket::acceptClient(void) const {
  sockaddr_in clientAddr;
  socklen_t addrLen = sizeof(clientAddr);

  std::memset(&clientAddr, 0, addrLen);
  int clientFd = accept(this->_socketFd, reinterpret_cast<struct sockaddr *>(&clientAddr), &addrLen);
  if (clientFd == -1) {
    std::perror("accept");
  }
  return (clientFd);
}

void ServerSocket::closeSocket(void) {
  if (this->_socketFd != -1) {
    ::close(this->_socketFd);
  }
}
