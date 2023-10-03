#include "../srcs/Message.hpp"
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

MU_TEST(parse_empty_message) {
  Message msg;

  msg = parseIrcMessage(std::string());

  mu_check(msg.prefix.data() == std::string());
  mu_check(msg.command.data() == std::string());
  mu_check(msg.params == std::vector<std::string>());
  mu_check(msg.trailingParam.data() == std::string());
}

MU_TEST_SUITE(unit_tests) { MU_RUN_TEST(parse_empty_message); }

MU_TEST_SUITE(integration_tests) {
  MU_SUITE_CONFIGURE(setupServer, teardownServer);
}

int main(void) {
  MU_RUN_SUITE(unit_tests);
  MU_RUN_SUITE(integration_tests);
  MU_REPORT();
  return (MU_EXIT_CODE);
}
