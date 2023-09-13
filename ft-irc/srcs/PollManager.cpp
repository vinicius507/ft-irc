#include "PollManager.hpp"
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <unistd.h>

PollManager::PollManager(void) : _pollFds() {}

PollManager::PollManager(const PollManager &other) : _pollFds(other._pollFds) {}

PollManager::~PollManager() { this->disconnectClients(); }

PollManager &PollManager::operator=(const PollManager &other) {
  if (this != &other) {
    this->disconnectClients();
    this->_pollFds = std::vector<struct pollfd>(other._pollFds);
  }
  return (*this);
}

void PollManager::disconnectClients(void) {
  std::vector<struct pollfd>::const_iterator it;

  if (this->_pollFds.size() <= 1) {
    return;
  }
  for (it = this->_pollFds.begin() + 1; it != this->_pollFds.end(); ++it) {
    ::close(it->fd);
  }
}

bool PollManager::hasNewClientOnQueue(void) const {
  return (this->_pollFds[0].revents & POLLIN);
}

std::vector<int> PollManager::getFdsReadyForReading(void) const {
  std::vector<int> fds;
  std::vector<struct pollfd>::const_iterator it;

  if (this->_pollFds.size() <= 1) {
    return (fds);
  }
  for (it = this->_pollFds.begin() + 1; it != this->_pollFds.end(); ++it) {
    if (it->revents & (POLLIN | POLLHUP)) {
      fds.push_back(it->fd);
    }
  }
  return (fds);
}

int PollManager::poll(void) {
  int pollStatus = ::poll(this->_pollFds.data(), this->_pollFds.size(), 1000);

  switch (pollStatus) {
  case -1:
    if (errno != EINTR) {
      std::perror("poll");
    }
    return (PERROR);
  case 0:
    return (PTIMEOUT);
  default:
    return (PEVENTS);
  }
}

void PollManager::addFd(int fd) {
  struct pollfd _pollFd;

  std::memset(&_pollFd, 0, sizeof(_pollFd));
  _pollFd.fd = fd;
  _pollFd.events = POLLIN;
  this->_pollFds.push_back(_pollFd);
}

void PollManager::removeFd(int fd) {
  std::vector<struct pollfd>::iterator it;

  for (it = this->_pollFds.begin(); it != this->_pollFds.end(); ++it) {
    if (it->fd == fd) {
      this->_pollFds.erase(it);
      ::close(fd);
      return;
    }
  }
}
