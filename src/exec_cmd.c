/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 12:06:26 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/09 10:07:11 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_signal.h"

static void	exec_child_process(t_node *node, t_shell *shell, char *path)
{
	if (handle_redirections(node->redir) != 0)
		exit(1);
	reset_signals();
	if (execve(path, node->argv, env_to_array(shell)) == -1)
	{
		perror("execve");
		free(path);
		exit(1);
	}
}

static int	exec_external_cmd(t_node *node, t_shell *shell, char *path)
{
	int		status;
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(path);
		return (1);
	}
	if (pid == 0)
		exec_child_process(node, shell, path);
	free(path);
	ignore_signals();
	waitpid(pid, &status, 0);
	return (get_exit_status(status));
}

int	execute_simple_command(t_node *node, t_shell *shell)
{
	char	*path;

	if (!node->argv || !node->argv[0])
		return (0);
	if (is_builtin(node->argv[0]))
	{
		if (node->redir)
			return (execute_builtin_redir(node->argv, node->redir, shell));
		return (execute_builtin(node->argv, shell));
	}
	path = get_path(node->argv[0], shell);
	if (!path)
	{
		ft_putstr_fd(node->argv[0], 2);
		ft_putendl_fd(": command not found", 2);
		return (127);
	}
	return (exec_external_cmd(node, shell, path));
}
