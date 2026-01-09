/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 13:25:17 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/09 10:11:47 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_red(char *str)
{
	int	tmp;

	tmp = dup(STDOUT_FILENO);
	dup2(STDERR_FILENO, STDOUT_FILENO);
	printf(RED "Err: %s" RESET "\n" , str);
	dup2(tmp, STDOUT_FILENO);
	close(tmp);
}

void	*safe_malloc(size_t __size)
{
	void	*res;

	res = ft_calloc(1, __size);
	if (!res)
		print_red("malloc fail.");
	return (res);
}
