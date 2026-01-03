/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 13:17:13 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/03 08:13:37 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exit(char **args, t_shell *shell)
{
	int	exit_code;

	printf("exit\n");
	if (args[1])
	{
		exit_code = ft_atoi(args[1]);
		clean_shell(shell);
		exit(exit_code);
	}
	clean_shell(shell);
	exit(shell->last_exit);
}
