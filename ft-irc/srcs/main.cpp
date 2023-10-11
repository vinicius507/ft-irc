#include "CliArgs.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include <csignal>
#include <cstring>
#include <iostream>
#include <sstream>

static inline void configureSignalHandlers(void) {
  struct sigaction mask;

  std::memset(&mask, 0, sizeof(struct sigaction));
  mask.sa_handler = Server::gracefulShutdown;
  sigaction(SIGINT, &mask, NULL);
  sigaction(SIGTERM, &mask, NULL);
  sigaction(SIGTSTP, &mask, NULL);
}

static bool parseArguments(int argc, char **argv, CliArgs &args) {
  int port;
  std::stringstream ss;

  if (argc != 3) {
    std::cerr << "Error: missing arguments." << std::endl;
    return (false);
  }
  ss.str(argv[1]);
  if (!(ss >> port) || ss.fail() || !ss.eof()) {
    std::cerr << "Error: invalid argument: " << argv[1] << std::endl;
    return (false);
  }
  args.port = port;
  if (std::strlen(argv[2]) < 1) {
    std::cerr << "Error: invalid argument: " << argv[2] << std::endl;
    return (false);
  }
  args.password = std::string(argv[2]);
  return (true);
}

int main(int argc, char **argv) {
  CliArgs args;
  Server server;

  if (!parseArguments(argc, argv, args)) {
    std::cerr << "Usage: " << argv[0] << " PORT PASSWORD" << std::endl;
    return (1);
  }
  server = Server(args.port);
  Client::setPassword(args.password);
  configureSignalHandlers();
  if (server.run() == false) {
    std::cerr << "Error: failed to run server." << std::endl;
    return (1);
  }
  return (0);
}
