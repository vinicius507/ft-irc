#include "Client.hpp"
#include <string>

Client::Client(void) : _nickname(), _usarname() {}

Client::Client(const std::string nickname, const std::string username)
    : _nickname(nickname), _usarname(username) {}

Client::Client(const Client &client) { *this = client; }

Client &Client::operator=(const Client &client) {
  if (this != &client) {
    this->_nickname = client._nickname;
    this->_usarname = client._usarname;
  }
  return (*this);
}

Client::~Client(void) {}

std::string Client::getName(void) { return (this->_usarname); }

std::string Client::getNick(void) { return (this->_nickname); }
