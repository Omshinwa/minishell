# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/30 15:04:13 by wiwu              #+#    #+#              #
#    Updated: 2026/01/02 19:18:29 by wiwu             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY: all clean fclean re bonus

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
LINK_FLAG = -lreadline

SRC_FILES	= main.c parse.c error.c
OBJ_FILES	= $(SRC_FILES:.c=.o)
SRC_PATH	= src/
OBJ_PATH	= obj/
INCLUDES	= include/
HEADER_FILES	= minishell.h minishell_parse.h

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
	rm -rf $(OBJ_PATH)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all
