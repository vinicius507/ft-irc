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

MU_TEST(parse_valid_message) {
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

MU_TEST(parse_empty_message) {
  Message msg;

  try {
    msg = parseIrcMessage(std::string());
    mu_assert(false, "Empty message should throw std::invalid_argument");
  } catch (std::invalid_argument &e) {
    mu_check(true);
  }
}

MU_TEST(parse_message_without_space) {
  Message msg;
  std::string message = ":lucas@capDoMato@hostPRIVMSG#canal1:eae,menormenor!";

  try {
    msg = parseIrcMessage(message);
    mu_assert(false, "Should throw std::invalid_argument");
  } catch (std::invalid_argument &e) {
    mu_check(true);
  }
}

MU_TEST(parse_prefix_only_user) {
  Message msg;
  std::string prefix = "lucas";
  std::string command = "PRIVMSG";
  std::vector<std::string> params;
  params.push_back("#canal1");
  std::string trailingParam = "eae, menor menor!";
  std::string message = ":lucas PRIVMSG #canal1 :eae, menor menor!";

  msg = parseIrcMessage(message);

  mu_assert_string_eq(msg.prefix.c_str(), prefix.c_str());
  mu_assert_string_eq(msg.command.c_str(), command.c_str());
  mu_assert_string_eq(msg.params.at(0).c_str(), params.at(0).c_str());
  mu_assert_string_eq(msg.trailingParam.c_str(), trailingParam.c_str());
}

MU_TEST(parse_command_with_number) {
  Message msg;
  std::string message = ":vini PRIVMS1 #canal1 :eae, menor menor!";

  try {
    msg = parseIrcMessage(message);
    mu_assert(false, "Should throw std::invalid_argument");
  } catch (std::invalid_argument &e) {
    mu_check(true);
  }
}

MU_TEST(parse_command_only_numbers) {
  Message msg;
  std::string message = ":meritissimo1!m1 213 #canal1 :eae, menor menor!";

  try {
    msg = parseIrcMessage(message);
    mu_assert(false, "Should throw std::invalid_argument");
  } catch (std::invalid_argument &e) {
    mu_check(true);
  }
}

MU_TEST(parse_command_without_space) {
  Message msg;
  std::string message = ":meritissimo1!m1 PRIVMSG#canal1:eae,menormenor!";

  try {
    msg = parseIrcMessage(message);
    mu_assert(false, "Should throw std::invalid_argument");
  } catch (std::invalid_argument &e) {
    mu_check(true);
  }
}

MU_TEST_SUITE(unit_tests) {
  MU_RUN_TEST(parse_empty_message);
  MU_RUN_TEST(parse_valid_message);
  MU_RUN_TEST(parse_message_without_space);
  MU_RUN_TEST(parse_prefix_only_user);
  MU_RUN_TEST(parse_command_with_number);
  MU_RUN_TEST(parse_command_only_numbers);
  MU_RUN_TEST(parse_command_without_space);
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
