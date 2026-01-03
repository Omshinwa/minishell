# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/30 15:04:13 by wiwu              #+#    #+#              #
#    Updated: 2026/01/03 18:32:44 by wiwu             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	minishell

CC			=	cc

CFLAGS		=	-Wall -Wextra -Werror -g

LINK_FLAG	=	-lreadline

RM			=	rm -rf

SRC_FILES	=	builtin_cd.c		\
				builtin_echo.c		\
				builtin_env.c		\
				builtin_exit.c		\
				builtin_export.c	\
				builtin_pwd.c		\
				builtin_unset.c		\
				builtins.c			\
				clean.c				\
				env_init.c			\
				env_set.c			\
				env_utils.c			\
				error.c				\
				main.c				\
				parse.c				\
				execute.c

OBJ_FILES	=	$(SRC_FILES:.c=.o)
SRC_PATH	=	src/
OBJ_PATH	=	obj/
INCLUDES	=	include/
HEADER_FILES =	minishell.h minishell_parse.h

LIBFT_DIR = libft
LIBFT_A = $(LIBFT_DIR)/libft.a

SOURCES	= $(addprefix $(SRC_PATH),$(SRC_FILES))
OBJECTS	= $(addprefix $(OBJ_PATH),$(OBJ_FILES))
HEADERS = $(addprefix $(INCLUDES),$(HEADER_FILES))

# RULES

all: $(NAME)

$(NAME): $(OBJECTS) $(LIBFT_A)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBFT_A) -I$(INCLUDES) $(LINK_FLAG) -o $(NAME)

$(OBJ_PATH)%.o: $(SRC_PATH)%.c $(HEADERS)
	mkdir -p $(OBJ_PATH)
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

.PHONY: all clean fclean re bonus
