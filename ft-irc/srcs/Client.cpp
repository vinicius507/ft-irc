#include "Client.hpp"
#include <string>
#include <unistd.h>

Client::Client(void) : _fd(-1) {}

Client::Client(int fd) : _fd(fd) {}

Client::Client(const Client &client) : _fd(client._fd) {}

Client &Client::operator=(const Client &client) {
  if (this != &client) {
    if (this->_fd != -1) {
      close(this->_fd);
    }
    const_cast<int &>(this->_fd) = client._fd;
  }
  return (*this);
}

Client::~Client(void) {
  if (this->_fd != -1) {
    close(this->_fd);
  }
}

int Client::getFd(void) const { return (this->_fd); }

std::string &Client::getBuffer(void) { return (this->_buffer); }
