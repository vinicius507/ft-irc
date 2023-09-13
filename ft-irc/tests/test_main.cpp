#include "../srcs/Server.hpp"
#include <cstdlib>
#include <iostream>
#include <minunit.h>
#include <signal.h>
#include <unistd.h>

pid_t serverPid = -1;

void setupServer(void) {
  Server server;

  serverPid = fork();
  mu_check(serverPid != -1);
  if (serverPid == 0) {
    server.run();
    std::exit(0);
  }
}

void teardownServer(void) {
  if (serverPid > 0) {
    kill(serverPid, SIGINT);
  }
}

MU_TEST_SUITE(integration_tests) {
  MU_SUITE_CONFIGURE(setupServer, teardownServer);
}

int main(void) {
  MU_RUN_SUITE(integration_tests);
  MU_REPORT();
  return (MU_EXIT_CODE);
}
