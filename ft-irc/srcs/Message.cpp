#include "Message.hpp"
#include <iostream>
#include <cstring>

Message parseIrcMessage(const std::string data) {
  Message msg;
  std::string comands[2] = {"NICK", "USER"};

  std::memset(&msg, 0, sizeof(Message));
  for (int i = 0; i < 2; ++i) {
    if (!data.compare(0, data.length() - 1, comands[i])) {
      std::cout << "command: " << comands[i] << std::endl;
      break;
    }
  }
  std::cerr << "Message from client: " << data << std::endl;
  return (msg);
}
