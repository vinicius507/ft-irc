#include "Server.hpp"
#include "Message.hpp"
#include "commands.hpp"
#include "numericReplies.hpp"
#include <algorithm>
#include <arpa/inet.h>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>

bool Server::_shouldExit = false;

Server::Server(void) : _pollFds(), _socket(), _connectionPassword() {}

Server::Server(short port, const std::string &connectionPassword)
    : _pollFds(), _socket(port), _connectionPassword(connectionPassword) {}

Server::Server(const Server &other)
    : _pollFds(other._pollFds), _socket(other._socket), _connectionPassword(other._connectionPassword) {}

Server::~Server(void) {}

Server &Server::operator=(const Server &other) {
  if (this != &other) {
    this->_pollFds = other._pollFds;
    this->_socket = other._socket;
    const_cast<std::string &>(this->_connectionPassword) = other._connectionPassword;
  }
  return (*this);
}

bool Server::run(void) {
  int clientFd;
  std::vector<int> arrayOfFds;
  std::vector<int>::iterator itOfFds;

  if (!this->_socket.bindAndListen()) {
    std::cerr << "Error: failed to create server TCP socket." << std::endl;
    return (false);
  }
  this->_pollFds.addFd(this->_socket.getFd());
  while (this->_shouldExit != true) {
    switch (this->_pollFds.poll()) {
    case PERROR:
      return (false);
    case PTIMEOUT:
      continue;
    default:
      break;
    }
    if (this->_pollFds.hasNewClientOnQueue()) {
      clientFd = this->_socket.acceptClient();
      if (clientFd != -1) {
        this->_clients.addClient(clientFd);
        this->_pollFds.addFd(clientFd);
      } else {
        std::cerr << "Error: couldn't accept new client" << std::endl;
      }
    }
    arrayOfFds = this->_pollFds.getFdsReadyForReading();
    for (itOfFds = arrayOfFds.begin(); itOfFds != arrayOfFds.end(); ++itOfFds) {
      this->handleClientData(*itOfFds);
    }
  }
  return (true);
}

void Server::handleMessage(Client *client, Message &msg) {
  if (msg.command == "PASS") {
    passCommand(*this, client, msg);
  } else if (msg.command == "NICK") {
    nickCommand(*this, client, msg);
  } else {
    if (client->getAuthState() != AuthDone) {
      client->send(ERR_NOTREGISTERED(client->getNickname()));
      return;
    }
    client->send(ERR_UNKNOWNCOMMAND(client->getNickname(), msg.command));
  }
}

bool Server::isConnectionPasswordValid(const std::string &connectionPassword) const {
  return (this->_connectionPassword == connectionPassword);
}

void Server::handleClientData(int clientFd) {
  Message msg;
  std::string::size_type crlf;
  Client *client = this->_clients[clientFd];

  switch (client->read()) {
  case Client::ReadError:
    std::cerr << "Error: couldn't read from client socket: " << std::strerror(errno) << std::endl;
    // Intentional fallthrough
  case Client::ReadEof:
    std::cerr << "INFO: Client disconnected: " << clientFd << std::endl;
    this->_pollFds.removeFd(clientFd);
    this->_clients.disconnectClient(clientFd);
    break;
  case Client::ReadIn:
    std::string &buf = client->getBuffer();
    crlf = buf.find(CRLF);
    if (crlf != std::string::npos) {
      try {
        msg = parseIrcMessage(std::string(buf.substr(0, crlf)));
        // handle message
      } catch (std::invalid_argument &e) {
        std::cerr << "Debug: Ignoring malformed message: " << e.what() << std::endl;
      } catch (std::exception &e) {
        std::cerr << "Error: Could not parse message: " << e.what() << std::endl;
      }
      buf.erase(0, crlf + CRLF.length());
    }
    break;
  }
}

Client *Server::getClientByNickname(const std::string &nickname) {
  ClientsManager::iterator it;

  for (it = this->_clients.begin(); it != this->_clients.end(); ++it) {
    if (it->second->getNickname() == nickname) {
      return (it->second);
    }
  }
  return (NULL);
}

void Server::gracefulShutdown(int signal) {
  Server::_shouldExit = true;
  std::cerr << "\nReceived signal " << signal << ", shutting down the server." << std::endl;
}
