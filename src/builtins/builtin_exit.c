/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 13:17:13 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/11 19:52:41 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exit(char **args, t_shell *shell)
{
	int	exit_code;

	ft_putendl_fd("exit", STDOUT_FILENO);
	if (args[1])
	{
		exit_code = ft_atoi(args[1]);
		clean_shell(shell);
		exit(exit_code);
	}
	exit_code = shell->last_exit;
	clean_shell(shell);
	exit(exit_code);
}
