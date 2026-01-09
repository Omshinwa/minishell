/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 13:17:30 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/03 09:09:06 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_cd(char **args, t_shell *shell)
{
	char	*path;

	if (!args[1])
	{
		path = getenv("HOME");
		if (!path)
		{
			ft_putendl_fd("cd: HOME not set", 2);
			shell->last_exit = 1;
			return (1);
		}
	}
	else
		path = args[1];
	if (chdir(path) == -1)
	{
		perror("cd");
		shell->last_exit = 1;
		return (1);
	}
	shell->last_exit = 0;
	return (0);
}
