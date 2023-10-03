#include "Message.hpp"
#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>
#include <stdexcept>

static std::string parseToken(const std::string data, std::size_t &start) {
  std::size_t end;
  std::string token;

  end = data.find(' ', start);
  if (end == std::string::npos) {
    token = data.substr(start);
    start = std::string::npos;
    return (token);
  }
  token = data.substr(start, (end + 1) - start);
  start = end + 1;
  return (token);
}

int isValidPrefix(std::string prefix) {
  std::size_t space = prefix.find(' ', 0);
  if (space == std::string::npos) {
    return (0);
  }
  return (1);
}

Message parseIrcMessage(const std::string data) {
  Message msg;
  std::size_t start;
  std::string token;

  if (data.empty()) {
    return (msg);
  }
  start = 0;
  if (data.at(0) == ':') {
    msg.prefix = parseToken(data, start);
    if (!isValidPrefix(msg.prefix)) {
      throw std::runtime_error("Not-Formated: prefix");
    }
  }
  msg.command = parseToken(data, start);
  while (start < data.length()) {
    if (data.at(start) == ':') {
      msg.trailingParam = data.substr(start);
      break;
    }
    msg.params.push_back(parseToken(data, start));
  }
  return (msg);
}
