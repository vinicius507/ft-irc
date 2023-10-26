#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#define NICKNAME_SPECIAL_CHARS "[]\\`_^{|}"

class Server;
class Client;
struct Message;

typedef void (*CommandFn)(Server &server, Client *client, Message &msg);

void passCommand(Server &server, Client *client, Message &msg);

void nickCommand(Server &server, Client *client, Message &msg);

void userCommand(Server &server, Client *client, Message &msg);

void joinCommand(Server &server, Client *client, Message &msg);

void quitCommand(Server &server, Client *client, Message &msg);

void partCommand(Server &server, Client *client, Message &msg);

#endif
