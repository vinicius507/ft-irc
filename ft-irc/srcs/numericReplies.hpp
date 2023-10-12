#ifndef NUMERICREPLIES_HPP
#define NUMERICREPLIES_HPP

#include <string>

#define numericReply(numeric, target, message) std::string(":ft-irc ") + numeric + " " + target + " " + message + CRLF

#define ERR_UNKNOWNCOMMAND(target, command) numericReply("421", target, command + " :Unknown command")
#define ERR_NOTREGISTERED(target) numericReply("451", target, ":You have not registered")
#define ERR_NEEDMOREPARAMS(target, command) numericReply("461", target, command + " :Not enough parameters")
#define ERR_ALREADYREGISTRED(target) numericReply("462", target, ":You may not reregister")
#define ERR_PASSWSMISMATCH(target) numericReply("464", target, ":Password Incorrect")

#endif
