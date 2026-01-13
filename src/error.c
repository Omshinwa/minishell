/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 13:25:17 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/13 13:02:53 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_3red(char *arg1, int i, char *arg3)
{
	char	*arg2;

	arg2 = ft_itoa(i);
	ft_putstr_fd(RED "Err: ", 2);
	ft_putstr_fd(arg1, 2);
	ft_putstr_fd(" ", 2);
	ft_putstr_fd(arg2, 2);
	free(arg2);
	ft_putstr_fd("  `", 2);
	ft_putstr_fd(arg3, 2);
	ft_putstr_fd("`", 2);
	ft_putstr_fd(RESET, 2);
	ft_putstr_fd("\n", 2);
}

// I would love to just use printf(RED "Err: %s" RESET "\n", str);
// But it seems like it doesnt work with some testers because of the stdio.h
// buffer system that doesnt fflush out when you dup2 the STD or something.
void	print_2red(char *arg1, char *arg2)
{
	ft_putstr_fd(RED "Err: ", 2);
	ft_putstr_fd(arg1, 2);
	ft_putstr_fd(" `", 2);
	ft_putstr_fd(arg2, 2);
	ft_putstr_fd("`", 2);
	ft_putstr_fd(RESET, 2);
	ft_putstr_fd("\n", 2);
}

void	print_red(char *str)
{
	ft_putstr_fd(RED "Err: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(RESET, 2);
	ft_putstr_fd("\n", 2);
}

void	*safe_malloc(size_t __size)
{
	void	*res;

	res = ft_calloc(1, __size);
	if (!res)
		print_red("malloc fail.");
	return (res);
}
