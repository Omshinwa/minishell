# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/30 15:04:13 by wiwu              #+#    #+#              #
#    Updated: 2026/01/15 11:45:48 by dasamuel         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

GREEN	= \033[0;32m
RED		= \033[0;31m
RESET	= \033[0m
TITLE	= \033[1;32m

NAME		=	minishell

CC			=	cc

CFLAGS		=	-Wall -Wextra -Werror -g3

LINK_FLAG	=	-lreadline

RM			=	rm -rf

SRC_FILES	=	builtins/builtin_cd.c				\
				builtins/builtin_echo.c				\
				builtins/builtin_env.c				\
				builtins/builtin_exit.c				\
				builtins/builtin_export.c			\
				builtins/builtin_pwd.c				\
				builtins/builtin_unset.c			\
				env/env_init.c						\
				env/env_set.c						\
				env/env_utils.c						\
				exec/exec_builtins.c				\
				exec/exec_cmd.c						\
				exec/exec_redir.c					\
				exec/exec_pipe.c					\
				exec/exec_tree.c					\
				exec/exec_utils.c					\
				expand/expand_utils.c				\
				expand/expand_utils2.c				\
				expand/expand_var.c					\
				expand/expand_node.c				\
				expand/expand_wildcards.c			\
				expand/expand_wildcards_utils.c		\
				expand/expand_wildcards_utils2.c	\
				clean.c								\
				error.c								\
				main.c								\
				parse/parse.c						\
				parse/multiline.c					\
				parse/token.c						\
				parse/token_internal.c				\
				parse/node.c						\
				parse/parse_cmd.c					\
				signal_set.c						\
				signal.c							\
				parse/get_next_line.c

OBJ_FILES	=	$(SRC_FILES:.c=.o)
SRC_PATH	=	src/
OBJ_PATH	=	obj/
INCLUDES	=	include/
HEADER_FILES =	minishell.h minishell_parse.h minishell_signal.h

LIBFT_DIR = libft
LIBFT_A = $(LIBFT_DIR)/libft.a

SOURCES	= $(addprefix $(SRC_PATH),$(SRC_FILES))
OBJECTS	= $(addprefix $(OBJ_PATH),$(OBJ_FILES))
HEADERS = $(addprefix $(INCLUDES),$(HEADER_FILES))

# RULES

all: $(NAME)

$(NAME): $(OBJECTS) $(LIBFT_A)
	@$(CC) $(CFLAGS) $(OBJECTS) $(LIBFT_A) $(LINK_FLAG) -o $(NAME)
	
	@echo "$(GREEN)✔ Successfully compiled$(RESET)"
	@echo

$(OBJ_PATH)%.o: $(SRC_PATH)%.c $(HEADERS)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -I$(INCLUDES) -c $< -o $@

$(LIBFT_A):

	@echo "$(TITLE)"
	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	@echo "  ███╗   ███╗ ██╗ ███╗   ██╗ ██╗ ███████╗ ██╗  ██╗ ███████╗ ██╗     ██╗"
	@echo "  ████╗ ████║ ██║ ████╗  ██║ ██║ ██╔════╝ ██║  ██║ ██╔════╝ ██║     ██║"
	@echo "  ██╔████╔██║ ██║ ██╔██╗ ██║ ██║ ███████╗ ███████║ █████╗   ██║     ██║"
	@echo "  ██║╚██╔╝██║ ██║ ██║╚██╗██║ ██║ ╚════██║ ██╔══██║ ██╔══╝   ██║     ██║"
	@echo "  ██║ ╚═╝ ██║ ██║ ██║ ╚████║ ██║ ███████║ ██║  ██║ ███████╗ ███████╗███████╗"
	@echo "  ╚═╝     ╚═╝ ╚═╝ ╚═╝  ╚═══╝ ╚═╝ ╚══════╝ ╚═╝  ╚═╝ ╚══════╝ ╚══════╝╚══════╝"
	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	@echo "$(RESET)Made by $(RED)wiwu$(RESET) and $(RED)dasamuel$(RESET)"
	@echo

	@$(MAKE) --silent -C $(LIBFT_DIR)
	
clean:
	@$(RM) $(OBJ_PATH)
	@$(MAKE) --silent -C $(LIBFT_DIR) clean
	
	@echo "$(GREEN)✔ Object files successfully deleted$(RESET)"
	@echo

fclean: clean
	@$(RM) $(NAME) $(NAME)_bonus
	@$(MAKE) --silent -C $(LIBFT_DIR) fclean

	@echo "$(GREEN)✔ Executable successfully deleted$(RESET)"
	@echo

re: fclean all

bonus: $(NAME)_bonus

$(NAME)_bonus: $(OBJECTS) $(LIBFT_A)
	@$(CC) $(CFLAGS) $(OBJECTS) $(LIBFT_A) $(LINK_FLAG) -o $(NAME)_bonus
	@echo "$(GREEN)✔ Successfully compiled$(RESET)"
	@echo

.PHONY: all clean fclean re bonus
