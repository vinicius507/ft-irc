#include "Server.hpp"
#include <csignal>
#include <iostream>

extern bool shouldExit;

void gracefulShutdown(int signal) {
  shouldExit = true;
  std::cerr << std::endl
            << "Received signal " << signal << ", shutting down the server."
            << std::endl;
}

int main(int argc, char **argv) {
  int port;

  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " PORT" << std::endl;
    return (1);
  }
  signal(SIGINT, gracefulShutdown);
  signal(SIGTERM, gracefulShutdown);
  port = std::atoi(argv[1]);
  Server server(port);
  if (!server.run()) {
    return (1);
  }
  return (0);
}
