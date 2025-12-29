# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/18 19:04:09 by dasamuel          #+#    #+#              #
#    Updated: 2025/12/19 00:33:38 by dasamuel         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	   =	pipex

CC		   =	cc -Wall -Wextra -Werror

RM		   =	rm -f

SRCS	   =	pipex.c		\
				execute.c	\
				utils.c		\
				ft_split.c

OBJS	   =	${SRCS:.c=.o}

all:	$(NAME)

$(NAME):	$(OBJS)
			$(CC) $(OBJS) -o $(NAME)

.c.o:
			$(CC) -c $< -o $@

clean:
			$(RM) $(OBJS)

fclean:		clean
			$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean re
