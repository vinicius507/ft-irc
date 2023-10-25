#include "Client.hpp"
#include <string>
#include <sys/socket.h>
#include <unistd.h>

Client::Client(void) : _fd(-1), _authState(AuthNone), _quitSent(false) {}

Client::Client(int fd) : _fd(fd), _authState(AuthNone), _quitSent(false) {}

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

AuthState Client::getAuthState(void) const { return (this->_authState); }

void Client::setAuthState(AuthState state) { this->_authState = state; }

int Client::getFd(void) const { return (this->_fd); }

std::string &Client::getBuffer(void) { return (this->_buffer); }

const std::string &Client::getNickname(void) const { return (this->_nickname); }

void Client::setNickname(const std::string &nickname) { const_cast<std::string &>(this->_nickname) = nickname; }
void Client::setUsername(const std::string &username) { const_cast<std::string &>(this->_username) = username; }
void Client::setRealname(const std::string &realname) { const_cast<std::string &>(this->_realname) = realname; }
void Client::setHostname(const std::string &hostname) { const_cast<std::string &>(this->_hostname) = hostname; }

const std::string &Client::getUsername(void) const { return (this->_username); }
const std::string &Client::getRealname(void) const { return (this->_realname); }
const std::string &Client::getHostname(void) const { return (this->_hostname); }

Client::ReadEvent Client::read(void) {
  ssize_t bytesRead;
  char buf[512] = {0};

  bytesRead = ::recv(this->_fd, buf, sizeof(buf), MSG_DONTWAIT);
  switch (bytesRead) {
  case -1:
    return (Client::ReadError);
  case 0:
    return (Client::ReadEof);
  default:
    this->_buffer += std::string(buf, buf + bytesRead);
    return (Client::ReadIn);
  }
}

void Client::send(const std::string &msg) { ::send(this->_fd, msg.c_str(), msg.size(), 0); }

void Client::quitSent(void) { this->_quitSent = true; }

bool Client::isQuitSent(void) const { return (this->_quitSent); }
