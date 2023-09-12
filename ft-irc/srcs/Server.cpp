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
    this->_pollFds = std::vector<struct pollfd>(other._pollFds);
    this->_socket = other._socket;
  }
  return (*this);
}

bool Server::run(void) {
  struct pollfd pollFd;

  if (!this->_socket.bindAndListen()) {
    std::cerr << "Error: failed to create server TCP socket." << std::endl;
    return (false);
  }
  std::memset(&pollFd, 0, sizeof(pollFd));
  pollFd.fd = this->_socket.getFd();
  pollFd.events = POLLIN;
  this->_pollFds.push_back(pollFd);
  while (!shouldExit) {
    int isReady = poll(this->_pollFds.data(), this->_pollFds.size(), 1000);
    if (isReady == -1) {
      return (false);
    }
    if (isReady == 0) {
      continue;
    }
    if (this->_pollFds[0].revents & POLLIN) {
      pollFd.fd = this->_socket.acceptClient();
      this->_pollFds.push_back(pollFd);
    }
    std::vector<struct pollfd>::const_iterator it;
    std::vector<struct pollfd> pollFds(this->_pollFds);
    for (it = pollFds.cbegin() + 1; it != pollFds.cend(); ++it) {
      if (it->revents & (POLLIN | POLLHUP)) {
        this->handleClientData(it->fd);
      }
    }
  }
  return (true);
}

void Server::removeClient(int clientFd) {
  std::vector<struct pollfd>::const_iterator it;

  for (it = this->_pollFds.cbegin(); it != this->_pollFds.cend(); ++it) {
    if (it->fd == clientFd) {
      this->_pollFds.erase(it);
      break;
    }
  }
  close(clientFd);
}

void Server::handleClientData(int clientFd) {
  int bytesRead;
  char buf[255] = {0};

  bytesRead = recv(clientFd, buf, sizeof(buf), MSG_DONTWAIT);
  if (bytesRead == -1) {
    std::perror("recv");
    this->removeClient(clientFd);
    return;
  }
  if (bytesRead == 0) {
    std::cerr << "Client disconnected: " << clientFd << std::endl;
    this->removeClient(clientFd);
    return;
  }
  std::string msg(buf, buf + bytesRead);
  std::cerr << "Message from client: " << msg << std::endl;
  send(clientFd, "\n", 1, 0);
}
