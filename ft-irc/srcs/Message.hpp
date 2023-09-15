#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <vector>

typedef struct _Message {
  std::string prefix;
  std::string command;
  std::vector<std::string> arguments;
} Message;

Message parseIrcMessage(const std::string data);

#endif
