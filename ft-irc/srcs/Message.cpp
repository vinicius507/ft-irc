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

bool checkCommand(std::string cmd) {
  size_t i;

  for (i = 0; i < cmd.length(); i++) {
    if (!isalpha(cmd[i])) {
      return (false);
    }
  }
  return (true);
}

bool isValidCommand(std::string command) {
  if (!checkCommand(command)) {
    return (false);
  }
  return (true);
}

bool isValidParam(std::string param) {
  if (param.find(':') != std::string::npos) {
    return (false);
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

static std::string parseParam(std::istringstream &iss) {
  std::string param;

  param = parseToken(iss);
  if (iss.eof()) {
    throw std::invalid_argument("No space after param");
  }
  if (!isValidParam(param)) {
    throw std::invalid_argument("Invalid param");
  }
  return (param);
}

Message parseIrcMessage(const std::string data) {
  Message msg;
  std::istringstream iss;

  if (data.empty()) {
    return (msg);
  }
  iss.str(data);
  if (data.at(0) == ':') {
    msg.prefix = parsePrefix(iss);
  }
  msg.command = parseCommand(iss);
  while (!iss.eof()) {
    if (iss.peek() == ':') {
      iss.ignore(1);
      msg.trailingParam =
          iss.str().substr(iss.tellg()); // Read the rest of the string buffer
      break;
    }
    msg.params.push_back(parseParam(iss));
  }
  return (msg);
}
