#include <iostream>
#include <cstring>
#include <poll.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

int main(void) {
  // Socket do servidor: usada pra aceitar conexões
  int socketFd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketFd == -1) {
    std::cout << "Error: socket creation failed\n";
    return (1);
  }
  // Associa o socketFd à porta 6667 usando o bind
  struct sockaddr_in sockaddress;
  sockaddress.sin_family = AF_INET;
  sockaddress.sin_addr.s_addr = INADDR_ANY;
  sockaddress.sin_port = htons(6667);
  int bindRes = bind(socketFd, (sockaddr *)&sockaddress, sizeof(sockaddress));
  if (bindRes < 0) {
    std::cout << "Error: bind creation failed" << std::endl;
    close(socketFd);
    return (1);
  }
  // Ouvindo na porta
  int listenRes = listen(socketFd, 10);
  if (listenRes == -1) {
    std::cout << "Error: couldn't listen on port" << std::endl;
    close(socketFd);
    return (1);
  }
  // Preparação pro loop principal
  struct pollfd fds[10];
  std::memset(fds, 0, 10 * sizeof(struct pollfd));
  fds[0].fd = socketFd;
  fds[0].revents = POLLIN;
  // Loop principal
    // ficaria monitorando cada fd?
  struct sockaddr_in addr;
  socklen_t addrLen;
  std::memset(&addr, 0, sizeof(addr));
  int newSocketFd = accept(socketFd, (sockaddr *)&addr, &addrLen);
  if (newSocketFd == -1) {
    std::cout << "Error: couldn't accept connection" << std::endl;
    close(socketFd);
    return (1);
  }
  fds[1].fd = newSocketFd;
  fds[1].revents = POLLIN;
  close(socketFd);
  close(fds[1].fd);
  return (0);
}