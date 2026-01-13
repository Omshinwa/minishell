/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 13:17:13 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/13 05:53:01 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_number(char *args)
{
	int	i;

	i = 0;
	if (args[i] == '+' || args[i] == '-')
		i++;
	if (!args[i])
		return (0);
	while (args[i])
	{
		if (!ft_isdigit(args[i]))
			return (0);
		i++;
	}
	return (1);
}

void	invalid_args(char *args, t_shell *shell)
{
	ft_putstr_fd("exit: ", STDERR_FILENO);
	ft_putstr_fd(args, STDERR_FILENO);
	ft_putendl_fd(": numeric argument required", STDERR_FILENO);
	clean_shell(shell);
	exit(2);
}

int	ft_exit(char **args, t_shell *shell)
{
	int	exit_code;

	ft_putendl_fd("exit", STDOUT_FILENO);
	if (args[1])
	{
		if (!is_valid_number(args[1]))
			invalid_args(args[1], shell);
		if (args[2])
		{
			ft_putendl_fd("exit: too many arguments", STDERR_FILENO);
			clean_shell(shell);
			exit(1);
		}
		exit_code = ft_atoi(args[1]);
		clean_shell(shell);
		exit(exit_code);
	}
	exit_code = shell->last_exit;
	clean_shell(shell);
	exit(exit_code);
}
