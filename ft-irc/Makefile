NAME = ircserv

CXX = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98

SRC_DIR := ./srcs
BUILD_DIR = ./build

SRCS := main.cpp

OBJS := $(addprefix $(BUILD_DIR)/,$(SRCS:%.cpp=%.o))
SRCS := $(addprefix $(SRC_DIR)/,$(SRCS))

all: ${NAME}

${NAME}: ${OBJS}
	${CXX} ${CFLAGS} $^ -o $@

${BUILD_DIR}/%.o: ${SRC_DIR}/%.cpp
	@mkdir -p ${BUILD_DIR}
	${CXX} ${CFLAGS} $< -c -o $@

clean:
	rm -rf ${BUILD_DIR}

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re
