# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/29 07:47:49 by dasamuel          #+#    #+#              #
#    Updated: 2025/12/29 08:21:31 by dasamuel         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	minishell

CC		=	cc -Wall -Wextra -Werror -I includes -I libft

RM		=	rm -rf

LIBFT	=	libft/libft.a

SRCS	=	

OBJS	=	$(SRCS:.c=.o)

.c.o:
			$(CC) -c $< -o $@
			
all:		$(NAME)

$(NAME):	$(OBJS) $(LIBFT)
			$(CC) $(OBJS) $(LIBFT) -o $(NAME)

$(LIBFT):
			make -C libft

clean:
			$(RM) $(OBJS)
			make clean -C libft

fclean:		clean
			$(RM) $(NAME)
			make fclean -C libft

re:			fclean all

.PHONY:		 all clean fclean re
