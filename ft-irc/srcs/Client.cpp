#include "Client.hpp"
#include <string>

Client::Client(void) : _nickname(), _username() {}

Client::Client(const std::string nickname, const std::string username)
    : _nickname(nickname), _username(username) {}

Client::Client(const Client &client) { *this = client; }

Client &Client::operator=(const Client &client) {
  if (this != &client) {
    this->_nickname = client._nickname;
    this->_username = client._username;
  }
  return (*this);
}

Client::~Client(void) {}

std::string Client::getNickName(void) { return (this->_nickname); }

std::string Client::getUserName(void) { return (this->_username); }
