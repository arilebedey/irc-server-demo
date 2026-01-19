NAME = ircserv
TEST_NAME = test_irc

SRC_DIR = src
INC_DIR = includes
OBJ_DIR = obj
TEST_OBJ_DIR = objt

SRCS =    client/client.cpp \
					server/server_utils.cpp \
					server/server_handler.cpp \
					server/server.cpp \
					command/command.cpp \
					command/handle_command.cpp \
					channel/channel.cpp \
					main.cpp \

TEST_SRCS = client/client.cpp \
						server/server_utils.cpp \
						server/server_handler.cpp \
						server/server.cpp \
						command/command.cpp \
						command/handle_command.cpp \
						channel/channel.cpp \
						test_main.cpp \

OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))
DEPS = $(OBJS:.o=.d)

TEST_OBJS = $(addprefix $(TEST_OBJ_DIR)/, $(TEST_SRCS:.cpp=.o))
TEST_DEPS = $(TEST_OBJS:.o=.d)

CC = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic -I$(INC_DIR) -MMD -MP
RM = rm -rf

all: $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)/server
	mkdir -p $(OBJ_DIR)/client
	mkdir -p $(OBJ_DIR)/command
	mkdir -p $(OBJ_DIR)/channel

$(TEST_OBJ_DIR):
	mkdir -p $(TEST_OBJ_DIR)/server
	mkdir -p $(TEST_OBJ_DIR)/client
	mkdir -p $(TEST_OBJ_DIR)/command
	mkdir -p $(TEST_OBJ_DIR)/channel
	mkdir -p $(TEST_OBJ_DIR)/tests

# Targets

$(NAME): $(OBJS)
	$(CC) $(CPPFLAGS) $(OBJS) -o $(NAME)

test: $(TEST_NAME)
	./$(TEST_NAME)

$(TEST_NAME): $(TEST_OBJS)
	$(CC) $(CPPFLAGS) $(TEST_OBJS) -o $(TEST_NAME)

# Obj construction

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) -c $< -o $@

$(TEST_OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(TEST_OBJ_DIR)
	$(CC) $(CPPFLAGS) -c $< -o $@

$(TEST_OBJ_DIR)/%.o: tests/%.cpp | $(TEST_OBJ_DIR)
	$(CC) $(CPPFLAGS) -c $< -o $@

# Helpers

clean:
	$(RM) $(OBJ_DIR) .cache

fclean: clean
	$(RM) $(NAME)

re: fclean all

-include $(DEPS)
-include $(TEST_DEPS)

.PHONY: all clean fclean re
