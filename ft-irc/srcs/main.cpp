#include "Server.hpp"
#include <csignal>
#include <iostream>

bool shouldExit = false;

void gracefulShutdown(int signal) {
  if (signal == SIGINT) {
    shouldExit = true;
  }
}

int main(int argc, char **argv) {
  int port;
  Server server;

  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " PORT" << std::endl;
    return (1);
  }
  port = std::stoi(argv[1]);
  server = Server(port);
  signal(SIGINT, gracefulShutdown);
  if (!server.run()) {
    return (1);
  }
  return (0);
}
