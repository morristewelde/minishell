# Variables
NAME        := minishell
CC          := gcc
CFLAGS      := -Wall -Wextra -Werror -Iinclude -Ilibft
LIBFT_DIR   := libft
LIBFT       := $(LIBFT_DIR)/libft.a
SRC_DIR     := src
OBJ_DIR     := obj

# Liste des fichiers sources
SRC_FILES   := main.c \
               signals.c \
               prompt.c\
               init.c\
               env.c\
               env_utils.c\
               utils.c\
               input.c\
               token_utils.c\
               tokenize.c\
               parser.c\
               utils_parser.c\
               quotes.c\
               validate_token.c\
               init_commands.c\
               organize_com_utils.c\
               organize_commands.c\
               expand_variables.c\
               handles_quotes_exp.c\
               cleanup.c\
               print_utils.c\
               debug.c\
               new_try_parsing.c\
               safe_utils.c
               #builtin_echo.c\
               #builtins.c\
               #env_array.c\
               #utils_b.c\
# Convertir les fichiers sources en chemins absolus
SRCS        := $(addprefix $(SRC_DIR)/, $(SRC_FILES))

# Conversion des fichiers sources en fichiers objets
OBJS        := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Règles
all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -L$(LIBFT_DIR) -lft -lreadline

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re

