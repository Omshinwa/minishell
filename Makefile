# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/30 15:04:13 by wiwu              #+#    #+#              #
#    Updated: 2025/12/29 11:01:16 by dasamuel         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY: all clean fclean re bonus

NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -pthread

SRC_FILES	= main.c parse.c init_table.c routine.c time.c error.c mutex_wrappers.c mutex_get_set.c
OBJ_FILES	= $(SRC_FILES:.c=.o)
SRC_PATH	= src/
OBJ_PATH	= obj/
INCLUDES	= include

SOURCES	= $(addprefix $(SRC_PATH),$(SRC_FILES))
OBJECTS	= $(addprefix $(OBJ_PATH),$(OBJ_FILES))

# RULES

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -I$(INCLUDES) -o $(NAME)

$(OBJ_PATH)%.o: $(SRC_PATH)%.c $(INCLUDES)/philo.h
	mkdir -p $(OBJ_PATH)
	$(CC) $(CFLAGS) -I$(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -f $(NAME)

re: fclean all
