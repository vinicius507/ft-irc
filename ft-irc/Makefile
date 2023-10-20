NAME = ircserv

CXX = c++
CFLAGS := -g3 -Wall -Wextra -Werror -std=c++98

SRC_DIR := ./srcs
BUILD_DIR = ./build
TESTS_DIR = ./tests

SRCS := main.cpp Server.cpp ServerSocket.cpp PollManager.cpp \
		Client.cpp Message.cpp ClientsManager.cpp commands/pass.cpp \
		commands/nick.cpp commands/user.cpp commands/join.cpp
TEST_SRCS := $(filter-out main.cpp,$(SRCS)) test_main.cpp

OBJS := $(addprefix $(BUILD_DIR)/,$(SRCS:%.cpp=%.o))
SRCS := $(addprefix $(SRC_DIR)/,$(SRCS))
TEST_OBJS := $(addprefix $(BUILD_DIR)/,$(TEST_SRCS:%.cpp=%.o))
TEST_SRCS := $(addprefix $(TESTS_DIR)/,$(TEST_SRCS))

all: ${NAME}

${NAME}: ${OBJS}
	${CXX} ${CFLAGS} ${OBJS} -o $@

tests: override CFLAGS = -g3 -O0 -fsanitize=address
tests: ${TEST_OBJS}
	${CXX} ${CFLAGS} ${TEST_OBJS} -lrt -lm -o run_tests
	./run_tests

${BUILD_DIR}/%.o: ${SRC_DIR}/%.cpp
	@mkdir -p $(dir $@)
	${CXX} ${CFLAGS} $< -c -o $@

${BUILD_DIR}/%.o: ${TESTS_DIR}/%.cpp
	@mkdir -p ${BUILD_DIR}
	${CXX} ${CFLAGS} $< -c -o $@

clean:
	rm -rf ${BUILD_DIR}

fclean: clean
	${RM} ${NAME}
	${RM} ./run_tests

re: fclean all

gdb: override CFLAGS = -g3 -O0
gdb: all
	gdb --tui --args $(NAME) 6667 senhaforte

tgdb: tests
	gdb --tui run_tests

.PHONY: all tests clean fclean re
