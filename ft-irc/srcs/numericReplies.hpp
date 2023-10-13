#ifndef NUMERICREPLIES_HPP
#define NUMERICREPLIES_HPP

#include <string>

#define numericReply(numeric, target, message) std::string(":ft-irc ") + numeric + " " + target + " " + message + CRLF

#define ERR_UNKNOWNCOMMAND(target, command) numericReply("421", target, command + " :Unknown command")
#define ERR_NONICKNAMEGIVEN(target) numericReply("431", target, ":No nickname given")
#define ERR_ERRONEUSNICKNAME(target, nickname) numericReply("432", target, nickname + " :Erroneous nickname")
#define ERR_NICKNAMEINUSE(target, nickname) numericReply("433", target, nickname + " :Nickname is already in use")
#define ERR_NOTREGISTERED(target) numericReply("451", target, ":You have not registered")
#define ERR_NEEDMOREPARAMS(target, command) numericReply("461", target, command + " :Not enough parameters")
#define ERR_ALREADYREGISTRED(target) numericReply("462", target, ":You may not reregister")
#define ERR_PASSWSMISMATCH(target) numericReply("464", target, ":Password Incorrect")

#endif
