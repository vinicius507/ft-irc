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

Server::Server(void) : _port(6667) {
  this->_listening = false;
  this->_pollFds = std::vector<struct pollfd>();
}

Server::Server(short port) : _port(port) {
  this->_listening = false;
  this->_pollFds = std::vector<struct pollfd>();
}

Server::Server(const Server &other) { *this = other; }

Server::~Server(void) { this->close(); }

Server &Server::operator=(const Server &other) {
  if (this != &other) {
    if (this->_listening) {
      this->close();
    }
    this->_listening = other._listening;
    this->_pollFds = std::vector<struct pollfd>(other._pollFds);
    this->_port = other._port;
  }
  return (*this);
}

void Server::close(void) {
  std::vector<pollfd>::iterator it;

  this->_listening = false;
  for (it = this->_pollFds.begin(); it != this->_pollFds.end(); it++) {
    ::close(it->fd);
  }
  this->_pollFds.clear();
}

bool Server::listen(void) {
  int bindResult, socketFd;
  struct pollfd serverSocket;
  struct sockaddr_in serverAddr;

  socketFd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketFd == -1) {
    std::perror("socket");
    return (false);
  }
  std::memset(&serverSocket, 0, sizeof(serverSocket));
  serverSocket.fd = socketFd;
  serverSocket.events = POLLIN;
  this->_pollFds.push_back(serverSocket);
  std::memset(&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = INADDR_ANY;
  serverAddr.sin_port = htons(this->_port);
  bindResult =
      bind(socketFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
  if (bindResult == -1) {
    std::perror("bind");
    return (false);
  }
  if (::listen(socketFd, 10) == -1) {
    std::perror("::listen");
    return (false);
  }
  std::cerr << "Started listening on 0.0.0.0:" << this->_port << "."
            << std::endl;
  this->_listening = true;
  return (true);
}

bool Server::run(void) {
  if (!this->listen()) {
    std::cerr << "Error: failed to create server TCP socket." << std::endl;
    return (false);
  }
  while (!shouldExit) {
    int isReady = poll(this->_pollFds.data(), this->_pollFds.size(), 1000);
    if (isReady == -1) {
      return (false);
    }
    if (isReady == 0) {
      continue;
    }
    if (this->_pollFds[0].revents & POLLIN) {
      this->acceptNewClient();
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

void Server::acceptNewClient(void) {
  int socketFd;
  socklen_t addrLen;
  struct pollfd pollFd;
  struct sockaddr_in addr;

  addrLen = sizeof(addr);
  std::memset(&addr, 0, addrLen);
  socketFd = accept(this->_pollFds[0].fd, (struct sockaddr *)&addr, &addrLen);
  if (socketFd == -1) {
    std::perror("accept");
    return;
  }
  std::memset(&pollFd, 0, sizeof(pollFd));
  pollFd.fd = socketFd;
  pollFd.events = POLLIN;
  this->_pollFds.push_back(pollFd);
  std::cerr << "New client connected: " << inet_ntoa(addr.sin_addr)
            << std::endl;
}

void Server::removeClient(int clientFd) {
  std::vector<struct pollfd>::const_iterator it;

  for (it = this->_pollFds.cbegin(); it != this->_pollFds.cend(); ++it) {
    if (it->fd == clientFd) {
      this->_pollFds.erase(it);
      break;
    }
  }
  ::close(clientFd);
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
