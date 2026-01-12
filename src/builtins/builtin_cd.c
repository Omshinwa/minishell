/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 13:17:30 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/12 10:54:59 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_args(char **args)
{
	int	len;

	len = 0;
	while (args[len])
		len++;
	return (len);
}

int	cd_with_path(char **args, t_shell *shell)
{
	char	*path;

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

int	ft_cd(char **args, t_shell *shell)
{
	int		len_args;
	char	*path;

	len_args = count_args(args);
	if (len_args > 2)
	{
		ft_putendl_fd("cd: too many arguments", 2);
		shell->last_exit = 1;
		return (1);
	}
	else if (!args[1])
	{
		path = getenv("HOME");
		if (!path)
		{
			ft_putendl_fd("cd: HOME not set", 2);
			shell->last_exit = 1;
			return (1);
		}
	}
	return (cd_with_path(args, shell));
}
