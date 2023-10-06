#include "Message.hpp"
#include <cctype>
#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <iterator>
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

bool isValidPrefix(std::string prefix) {
  if (prefix.empty()) {
    return (false);
  }
  return (true);
}

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

bool isValidCommand(std::string command) {
  std::string::iterator it;

  for (it = command.begin(); it != command.end(); ++it) {
    if (!std::isalpha(*it)) {
      return (false);
    }
  }
  return (true);
}

static std::string parseCommand(std::istringstream &iss) {
  std::string command;

  command = parseToken(iss);
  if (iss.eof()) {
    throw std::invalid_argument("No space after command");
  }
  if (!isValidCommand(command)) {
    throw std::invalid_argument("Invalid command");
  }
  return (command.substr(0));
}

Message parseIrcMessage(const std::string data) {
  Message msg;
  std::istringstream iss;

  if (data.empty()) {
    throw std::invalid_argument("Empty message");
  }
  iss.str(data);
  if (data.at(0) == ':') {
    msg.prefix = parsePrefix(iss);
  }
  msg.command = parseCommand(iss);
  while (!iss.eof()) {
    if (iss.peek() == ':') {
      iss.ignore(1);
      msg.trailingParam = iss.str().substr(iss.tellg()); // Read the rest of the string buffer
      break;
    }
    msg.params.push_back(parseToken(iss));
  }
  return (msg);
}
