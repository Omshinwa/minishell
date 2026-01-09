/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 10:33:30 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/09 17:11:18 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	save_fds(int *saved_stdin, int *saved_stdout)
{
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	if (*saved_stdin == -1 || *saved_stdout ==-1)
	{
		if (*saved_stdin != -1)
			close(*saved_stdin);
		if (*saved_stdout != -1)
			close(*saved_stdout);
		return (1);
	}
	return (0);
}

static void	restore_fds(int saved_stdin, int saved_stdout)
{
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

int	execute_builtin_redir(char **argv, t_redir *redir, t_shell *shell)
{
	int	saved_stdin;
	int	saved_stdout;
	int	status;

	if (save_fds(&saved_stdin, &saved_stdout))
		return (1);
	if (handle_redirections(redir) != 0)
	{
		restore_fds(saved_stdin, saved_stdout);
		return (1);
	}
	status = execute_builtin(argv, shell);
	restore_fds(saved_stdin, saved_stdout);
	return (status);
}
