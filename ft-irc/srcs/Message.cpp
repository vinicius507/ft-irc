#include "Message.hpp"
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>

Message parseIrcMessage(const std::string data) {
  Message msg;
  std::string token;
  std::stringstream ss(data);

  if (data.at(0) == ':') {
    std::getline(ss, msg.prefix, ss.widen(' '));
    std::cout << "\n-prefix-\n";
    if (ss.fail()) {
      std::perror("getline");
      return (msg);
    }
  }
  std::getline(ss, msg.command, ss.widen(' '));
  while (!ss.eof()) {
    std::getline(ss, token, ss.widen(' '));
    if (token.at(0) == ':') {
      msg.trailingParam = token + ss.str();
      break;
    }
    msg.params.push_back(token);
  }
  return (msg);
}
