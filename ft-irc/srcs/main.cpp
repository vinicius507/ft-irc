#include "Client.hpp"
#include "Server.hpp"
#include <csignal>
#include <iostream>
#include <sstream>

static inline void configureSignalHandlers(void) {
  struct sigaction mask;
  mask.sa_handler = Server::gracefulShutdown;

  sigaction(SIGINT, &mask, NULL);
  sigaction(SIGTERM, &mask, NULL);
  sigaction(SIGTSTP, &mask, NULL);
}

static int parseArguments(int argc, char **argv) {
  int port;
  std::stringstream ss;

  if (argc != 2) {
    std::cerr << "Error: missing arguments." << std::endl;
    return (-1);
  }
  ss.str(argv[1]);
  if (!(ss >> port) || ss.fail() || !ss.eof()) {
    std::cerr << "Error: invalid argument: " << argv[1] << std::endl;
    return (-1);
  }
  return (port);
}

int main(int argc, char **argv) {
  int port;
  Server server;

  port = parseArguments(argc, argv);
  if (port == -1) {
    std::cerr << "Usage: " << argv[0] << " PORT" << std::endl;
    return (1);
  }
  server = Server(port);
  configureSignalHandlers();
  if (server.run() == false) {
    std::cerr << "Error: failed to run server." << std::endl;
    return (1);
  }
  return (0);
}
