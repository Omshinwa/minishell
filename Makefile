# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/30 15:04:13 by wiwu              #+#    #+#              #
#    Updated: 2026/01/10 09:14:46 by dasamuel         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	minishell

CC			=	cc

CFLAGS		=	-Wall -Wextra -Werror -g3 -D_DEFAULT_SOURCE

LINK_FLAG	=	-lreadline

RM			=	rm -rf

SRC_FILES	=	builtins/builtin_cd.c				\
				builtins/builtin_echo.c				\
				builtins/builtin_env.c				\
				builtins/builtin_exit.c				\
				builtins/builtin_export.c			\
				builtins/builtin_pwd.c				\
				builtins/builtin_unset.c			\
				builtins/builtins.c					\
				exec/exec_builtins.c				\
				exec/exec_cmd.c						\
				exec/exec_redir.c					\
				exec/exec_pipe.c					\
				exec/exec_tree.c					\
				exec/exec_utils.c					\
				expand/expand_utils.c				\
				expand/expand_var.c					\
				expand/expand_node.c				\
				expand/expand_wildcards.c			\
				expand/expand_wildcards_utils.c		\
				expand/expand_wildcards_utils2.c	\
				clean.c								\
				env_init.c							\
				env_set.c							\
				env_utils.c							\
				error.c								\
				main.c								\
				parse/parse.c						\
				parse/multilines.c					\
				parse/token.c						\
				parse/token_internal.c				\
				parse/node.c						\
				parse/parse_cmd.c					\
				signal.c

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
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBFT_A) $(LINK_FLAG) -o $(NAME)

$(OBJ_PATH)%.o: $(SRC_PATH)%.c $(HEADERS)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INCLUDES) -c $< -o $@

$(LIBFT_A):
	$(MAKE) -C $(LIBFT_DIR)
	
clean:
	$(RM) $(OBJ_PATH)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
