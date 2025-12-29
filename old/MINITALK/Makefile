# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/30 15:04:13 by wiwu              #+#    #+#              #
#    Updated: 2025/12/15 18:13:40 by wiwu             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY: all clean fclean re bonus

SERVER = server
CLIENT = client

CC = cc
CFLAGS = -Wall -Wextra -Werror 

LIBFT_DIR = libft
LIBFT_A = $(LIBFT_DIR)/libft.a

INCLUDES = -I$(LIBFT_DIR)

SOURCES_SERVER = server.c
SOURCES_CLIENT = client.c

OBJECTS_SERVER = $(SOURCES_SERVER:.c=.o)
OBJECTS_CLIENT = $(SOURCES_CLIENT:.c=.o)

# BONUS?

SERVER_BONUS = $(SERVER)_bonus
CLIENT_BONUS = $(CLIENT)_bonus

# RULES

all: $(SERVER) $(CLIENT)

bonus: $(SERVER_BONUS) $(CLIENT_BONUS)

$(LIBFT_A):
	$(MAKE) -C $(LIBFT_DIR)

$(SERVER): $(OBJECTS_SERVER) $(LIBFT_A)
	$(CC) $(CFLAGS) $(OBJECTS_SERVER) $(LIBFT_A) -o $(SERVER)

$(CLIENT): $(OBJECTS_CLIENT) $(LIBFT_A)
	$(CC) $(CFLAGS) $(OBJECTS_CLIENT) $(LIBFT_A) -o $(CLIENT)

# BONUS?
$(SERVER_BONUS): $(OBJECTS_SERVER) $(LIBFT_A)
	$(CC) $(CFLAGS) $(OBJECTS_SERVER) $(LIBFT_A) -o $(SERVER_BONUS)

$(CLIENT_BONUS): $(OBJECTS_CLIENT) $(LIBFT_A)
	$(CC) $(CFLAGS) $(OBJECTS_CLIENT) $(LIBFT_A) -o $(CLIENT_BONUS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJECTS_SERVER)
	rm -f $(OBJECTS_CLIENT)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(SERVER)
	rm -f $(CLIENT)
	rm -f $(SERVER_BONUS)
	rm -f $(CLIENT_BONUS)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all