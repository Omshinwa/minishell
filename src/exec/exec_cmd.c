/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 12:06:26 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/14 08:51:21 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_child_process(t_node *node, t_shell *shell, char *path)
{
	if (handle_redirections(node->redir) != 0)
	{
		clean_shell(shell);
		exit(1);
	}
	reset_signals();
	if (execve(path, node->argv, env_to_array(shell)) == -1)
	{
		perror("execve");
		free(path);
		clean_shell(shell);
		exit(127);
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

static int	print_error(char *cmd, int n)
{
	if (n == 1)
	{
		ft_putstr_fd(cmd, 2);
		ft_putendl_fd(": command not found", 2);
		return (127);
	}
	else if (n == 2)
	{
		ft_putstr_fd(cmd, 2);
		ft_putendl_fd(": Is a directory", 2);
		return (126);
	}
	else if (n == 3)
	{
		ft_putstr_fd(cmd, 2);
		ft_putendl_fd(": Permission denied", 2);
		return (126);
	}
	return (1);
}

int	execute_simple_command(t_node *node, t_shell *shell)
{
	char		*path;
	struct stat	path_stat;
	int			expand_fail;

	expand_fail = expand_cmd_node(node, shell);
	if (expand_fail)
		return (1);
	if (is_builtin(node->argv[0]))
	{
		if (node->redir)
			return (execute_builtin_redir(node->argv, node->redir, shell));
		return (execute_builtin(node->argv, shell));
	}
	path = get_path(node->argv[0], shell);
	if (!path || !node->argv[0][0])
		return (print_error(node->argv[0], 1));
	if (stat(path, &path_stat) == -1)
		return (print_error(node->argv[0], 1));
	if (S_ISDIR(path_stat.st_mode))
		return (print_error(node->argv[0], 2));
	if (access(path, X_OK) != 0)
		return (print_error(node->argv[0], 3));
	return (exec_external_cmd(node, shell, path));
}
