#include "Server.hpp"
#include "Channel.hpp"
#include "Message.hpp"
#include "commands.hpp"
#include "serverReplies.hpp"
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
        this->handleNewClient(clientFd);
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

bool Server::isConnectionPasswordValid(const std::string &connectionPassword) const {
  return (this->_connectionPassword == connectionPassword);
}

void Server::handleNewClient(int clientFd) {
  this->_clients.addClient(clientFd);
  this->_pollFds.addFd(clientFd);
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
    this->disconnectClient(client);
    break;
  case Client::ReadIn:
    std::string &buf = client->getBuffer();
    while ((crlf = buf.find(CRLF)) != std::string::npos) {
      std::string data = buf.substr(0, crlf);
      std::cerr << "Debug: Parsing message: " << data << std::endl;
      buf.erase(0, crlf + CRLF.length());
      try {
        msg = parseIrcMessage(data);
        this->handleMessage(client, msg);
        if (client->isQuitSent()) {
          this->disconnectClient(client);
          break;
        }
      } catch (std::invalid_argument &e) {
        std::cerr << "Debug: Ignoring malformed message: " << e.what() << std::endl;
      } catch (std::exception &e) {
        std::cerr << "Error: Could not parse message: " << e.what() << std::endl;
      }
    }
    break;
  }
}

void Server::disconnectClient(Client *client) {
  std::map<std::string, Channel *>::iterator it;

  for (it = this->_channels.begin(); it != this->_channels.end();) {
    it->second->removeClient(client);
    if (it->second->getClientsCount() == 0) {
      delete it->second;
      this->_channels.erase(it++);
    } else {
      ++it;
    }
  }
  this->_pollFds.removeFd(client->getFd());
  this->_clients.disconnectClient(client->getFd());
}

void Server::handleMessage(Client *client, Message &msg) {
  CommandFn cmdHandler;

  cmdHandler = getCommandHandler(msg);
  if (cmdHandler == NULL) {
    if (client->getAuthState() != AuthDone) {
      client->send(ERR_NOTREGISTERED("*"));
      return;
    }
    client->send(ERR_UNKNOWNCOMMAND(client->getNickname(), msg.command));
    return;
  }
  cmdHandler(*this, client, msg);
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

Channel *Server::getChannel(std::string &channelName) {
  std::map<std::string, Channel *>::iterator it;

  it = this->_channels.find(channelName);
  if (it == this->_channels.end()) {
    return (NULL);
  }
  return (it->second);
}

std::vector<Channel *> Server::getChannelsWithClient(Client *client) const {
  std::vector<Channel *> channels;
  std::map<std::string, Channel *>::const_iterator it;

  if (client == NULL) {
    return (channels);
  }
  for (it = this->_channels.begin(); it != this->_channels.end(); ++it) {
    if (it->second->hasClient(client)) {
      channels.push_back(it->second);
    }
  }
  return (channels);
}

void Server::createChannel(const std::string &channelName, const std::string &key, Client *client) {
  Channel *channel = new Channel(channelName);
  channel->setKey(key);
  channel->addClient(client);
  this->_channels.insert(std::pair<std::string, Channel *>(channelName, channel));
}

void Server::sendToVisible(Client *client, const std::string &message) {
  std::vector<Channel *> channels;
  std::vector<Channel *>::iterator it;

  channels = this->getChannelsWithClient(client);
  for (it = channels.begin(); it != channels.end(); ++it) {
    (*it)->sendToVisible(client, message);
  }
}
