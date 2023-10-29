#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#define NICKNAME_SPECIAL_CHARS "[]\\`_^{|}"

class Server;
class Client;
struct Message;

void passCommand(Server &server, Client *client, Message &msg);

void nickCommand(Server &server, Client *client, Message &msg);

void userCommand(Server &server, Client *client, Message &msg);

void joinCommand(Server &server, Client *client, Message &msg);

void pingCommand(Server &server, Client *client, Message &msg);

void quitCommand(Server &server, Client *client, Message &msg);

void partCommand(Server &server, Client *client, Message &msg);

void privmsgCommand(Server &server, Client *client, Message &msg);

void noticeCommand(Server &server, Client *client, Message &msg);

void kickCommand(Server &server, Client *client, Message &msg);

void topicCommand(Server &server, Client *client, Message &msg);

void inviteCommand(Server &server, Client *client, Message &msg);

typedef void (*CommandFn)(Server &server, Client *client, Message &msg);

enum Command {
  CMD_PASS,
  CMD_NICK,
  CMD_USER,
  CMD_JOIN,
  CMD_PING,
  CMD_QUIT,
  CMD_PART,
  CMD_PRIVMSG,
  CMD_NOTICE,
  CMD_KICK,
  CMD_TOPIC,
  CMD_INVITE,
  CMD_COUNT,  
};

CommandFn getCommandHandler(Message &msg);

#endif
