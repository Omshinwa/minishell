/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 13:17:13 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/14 21:07:50 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_atol_safe(char *s, long *res)
{
	long	v;
	int		sign;

	v = 0;
	sign = 1;
	if (*s == '+' || *s == '-')
		if (*s++ == '-')
			sign = -1;
	if (!*s)
		return (0);
	while (*s)
	{
		if (!ft_isdigit(*s))
			return (0);
		if (v > (LONG_MAX - (*s - '0')) / 10)
			return (0);
		v = v * 10 + (*s++ - '0');
	}
	*res = v * sign;
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
	long	exit_code;

	exit_code = shell->last_exit;
	// ft_putendl_fd("exit", STDOUT_FILENO);
	if (args[1])
	{
		if (!ft_atol_safe(args[1], &exit_code))
			invalid_args(args[1], shell);
		if (args[2])
		{
			ft_putendl_fd("exit: too many arguments", STDERR_FILENO);
			clean_shell(shell);
			exit(1);
		}
		exit_code = ft_atoi(args[1]);
		clean_shell(shell);
		exit((unsigned char)exit_code);
	}
	exit_code = shell->last_exit;
	clean_shell(shell);
	exit(exit_code);
}
