#include "commands.hpp"

#include "Client.hpp"
#include "Message.hpp"
#include <string>

static const std::string cmdNames[CMD_COUNT] = {
    "PASS", "NICK", "USER", "JOIN", "PING", "QUIT", "PART", "PRIVMSG", "NOTICE", "KICK", "TOPIC"};

static const CommandFn _commands[CMD_COUNT] = {
    passCommand,
    nickCommand,
    userCommand,
    joinCommand,
    pingCommand,
    quitCommand,
    partCommand,
    privmsgCommand,
    noticeCommand,
    kickCommand,
    topicCommand,
};

CommandFn getCommandHandler(Message &msg) {
  Command cmd = CMD_COUNT;

  for (int i = 0; i < CMD_COUNT; ++i) {
    if (cmdNames[i] == msg.command) {
      cmd = static_cast<Command>(i);
      break;
    }
  }
  if (cmd == CMD_COUNT) {
    return (NULL);
  }
  return (_commands[cmd]);
}
