#include "../srcs/Message.hpp"
#include "../srcs/Server.hpp"
#include <cstdlib>
#include <exception>
#include <iostream>
#include <minunit.h>
#include <signal.h>
#include <string>
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

MU_TEST(parse_simple_message) {
  Message msg;
  std::string prefix = "lucas@capDoMato@host";
  std::string command = "PRIVMSG";
  std::vector<std::string> params;
  params.push_back("#canal1");
  std::string trailingParam = "eae, menor menor!";
  std::string message =
      ":lucas@capDoMato@host PRIVMSG #canal1 :eae, menor menor!";

  msg = parseIrcMessage(message);

  mu_assert_string_eq(msg.prefix.c_str(), prefix.c_str());
  mu_assert_string_eq(msg.command.c_str(), command.c_str());
  mu_assert_string_eq(msg.params.at(0).c_str(), params.at(0).c_str());
  mu_assert_string_eq(msg.trailingParam.c_str(), trailingParam.c_str());
}

MU_TEST(parse_message_without_space) {
  Message msg;

  try {
    msg =
        parseIrcMessage(":lucas@capDoMato@hostPRIVMSG#canal1:eae,menormenor!");
    mu_assert(false, "Should throw std::invalid_argument");
  } catch (std::invalid_argument &e) {
    mu_check(true);
  }
}

MU_TEST_SUITE(unit_tests) {
  MU_RUN_TEST(parse_empty_message);
  MU_RUN_TEST(parse_simple_message);
  MU_RUN_TEST(parse_message_without_space);
}

MU_TEST_SUITE(integration_tests) {
  MU_SUITE_CONFIGURE(setupServer, teardownServer);
}

int main(void) {
  MU_RUN_SUITE(unit_tests);
  MU_RUN_SUITE(integration_tests);
  MU_REPORT();
  return (MU_EXIT_CODE);
}
