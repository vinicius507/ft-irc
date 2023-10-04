#include "Message.hpp"
#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>
#include <stdexcept>

static std::string parseToken(std::istringstream &iss) {
  std::string token;

  std::getline(iss, token, iss.widen(' '));
  if (iss.fail()) {
    throw std::runtime_error("Failed to parse token");
  }
  return (token);
}

bool isValidPrefix(std::string prefix) { return (!prefix.empty()); }

static std::string parsePrefix(std::istringstream &iss) {
  std::string prefix;

  prefix = parseToken(iss);
  if (iss.eof()) {
    throw std::invalid_argument("No space after prefix");
  }
  if (!isValidPrefix(prefix)) {
    throw std::invalid_argument("Invalid prefix");
  }
  return (prefix.substr(1));
}

Message parseIrcMessage(const std::string data) {
  Message msg;
  std::istringstream iss;

  if (data.empty()) {
    return (msg);
  }
  iss.str(data);
  std::cerr << "Msg: '" << data << "'" << std::endl;
  if (data.at(0) == ':') {
    msg.prefix = parsePrefix(iss);
    std::cerr << "Prefix: '" << msg.prefix << "'" << std::endl;
  }
  msg.command = parseToken(iss);
  std::cerr << "Command: '" << msg.command << "'" << std::endl;
  while (!iss.eof()) {
    if (iss.peek() == ':') {
      iss.ignore(1);
      msg.trailingParam =
          iss.str().substr(iss.tellg()); // Read the rest of the string buffer
      std::cerr << "Trailing param: '" << msg.trailingParam << "'" << std::endl;
      break;
    }
    msg.params.push_back(parseToken(iss));
    std::cerr << "Param: '" << msg.params.back() << "'" << std::endl;
  }
  return (msg);
}
