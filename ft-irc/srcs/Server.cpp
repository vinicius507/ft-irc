#include "Server.hpp"
#include <algorithm>
#include <arpa/inet.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>

extern bool shouldExit;

Server::Server(void) : _pollFds(), _socket() {}

Server::Server(short port) : _pollFds(), _socket(port) {}

Server::Server(const Server &other) { *this = other; }

Server::~Server(void) {}

Server &Server::operator=(const Server &other) {
  if (this != &other) {
    this->_pollFds = other._pollFds;
    this->_socket = other._socket;
  }
  return (*this);
}

bool Server::run(void) {
  int fd;
  std::vector<int> fds;
  std::vector<int>::const_iterator it;

  if (!this->_socket.bindAndListen()) {
    std::cerr << "Error: failed to create server TCP socket." << std::endl;
    return (false);
  }
  this->_pollFds.addFd(this->_socket.getFd());
  while (!shouldExit) {
    int pollStatus = this->_pollFds.poll();
    if (pollStatus == PERROR) {
      std::cerr << "Error: failed to poll TCP sockets." << std::endl;
      return (false);
    }
    if (pollStatus == PTIMEOUT) {
      continue;
    }
    if (this->_pollFds.hasNewClientOnQueue()) {
      fd = this->_socket.acceptClient();
      this->_pollFds.addFd(fd);
    }
    fds = this->_pollFds.getFdsReadyForReading();
    for (it = fds.begin(); it != fds.end(); ++it) {
      this->handleClientData(*it);
    }
  }
  return (true);
}

void Server::handleClientData(int clientFd) {
  int bytesRead;
  char buf[255] = {0};

  bytesRead = recv(clientFd, buf, sizeof(buf), MSG_DONTWAIT);
  if (bytesRead == -1) {
    std::perror("recv");
    this->_pollFds.removeFd(clientFd);
    return;
  }
  if (bytesRead == 0) {
    std::cerr << "Client disconnected: " << clientFd << std::endl;
    this->_pollFds.removeFd(clientFd);
    return;
  }
  std::string msg(buf, buf + bytesRead);
  std::cerr << "Message from client: " << msg << std::endl;
  send(clientFd, "\n", 1, 0);
}
