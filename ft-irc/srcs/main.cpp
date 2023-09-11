#include <iostream>
#include <cstring>
#include <poll.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <csignal>

int interrupted = 0; // variavel de controle. Podemos deixar como atributo de alguma classe

void closeAllSockets(struct pollfd *fds, nfds_t n) {
  for (nfds_t i = 0; i < n; i++) {
    if (fds[i].fd > 0) {
      close(fds[i].fd);
      fds[i].fd = 0;
    }
  }
}

void signalHandler(int signal) {
	if (signal == SIGINT)
		interrupted = 1;
	std::cout << "\033[2K\r";
}

int main(void) {
	
  // manipulando o ^C
  signal(SIGINT, signalHandler);

  // Socket do servidor: usada pra aceitar conexões
  int socketFd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketFd == -1) {
    std::cerr << "Error: socket creation failed\n";
    return (1);
  }
  // Associa o socketFd à porta 6667 usando o bind
  struct sockaddr_in sockaddress;
  sockaddress.sin_family = AF_INET;
  sockaddress.sin_addr.s_addr = INADDR_ANY;
  sockaddress.sin_port = htons(6667);
  int bindRes = bind(socketFd, (sockaddr *)&sockaddress, sizeof(sockaddress));
  if (bindRes < 0) {
    std::cerr << "Error: bind creation failed" << std::endl;
    close(socketFd);
    return (1);
  }
  // Ouvindo na porta
  int listenRes = listen(socketFd, 10);
  if (listenRes == -1) {
    std::cerr << "Error: couldn't listen on port" << std::endl;
    close(socketFd);
    return (1);
  }
  // Preparação pro loop principal
  struct pollfd fds[10];
  std::memset(fds, 0, 10 * sizeof(struct pollfd));
  fds[0].fd = socketFd;
  fds[0].events = POLLIN;
  socklen_t addrLen;
  struct sockaddr_in addr;
  std::memset(&addr, 0, sizeof(addr));
  while (!interrupted) {
    int isReady = poll(fds, 10, 1000);
    if (isReady < 0) {
      closeAllSockets(fds, 10);
      return (1);
    }
    if (fds[0].revents & POLLIN) {
      std::cerr << "Info: New client trying to connect" << std::endl;
      int newSocketFd = accept(socketFd, (sockaddr *)&addr, &addrLen);
      if (newSocketFd < 0) {
        std::cerr << "Warn: Could not connect to new client" << std::endl;
      }
      int i;
      for (i = 1; i < 10; i++) {
        if (fds[i].fd == 0) {
          fds[i].fd = newSocketFd;
          fds[i].events = POLLIN;
          break;
        }
      }
      if (i == 10) {
        std::cerr << "Error: maximum number of clients was reached" << std::endl;
        close(newSocketFd);
      }
    }
    for (int i = 1; i < 10; i++) {
      if (fds[i].fd != 0 && fds[i].revents & POLLIN) {
        char buf[255] = {0};
        int bytesRead = recv(fds[i].fd, buf, sizeof(buf), 0);
        if (bytesRead <= 0) {
          std::cerr << "Info: Client disconnected" << std::endl;
          close(fds[i].fd);
          fds[i].fd = 0;
        } else {
          if (std::strncmp(buf, "gg", std::max(2, bytesRead)) == 0) {
            std::cerr << "Info: ggwp" << std::endl;
            closeAllSockets(fds, 10);
            return (1);
          }
          std::string msg(buf, buf + bytesRead);
          std::cerr << "Info: received message from client " << i << ": " << msg << std::endl;
          send(fds[i].fd, buf, bytesRead, 0);
        }
      }
    }
  }
  closeAllSockets(fds, 10);
  return (0);
}
