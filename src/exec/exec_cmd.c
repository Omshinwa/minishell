/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 12:06:26 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/13 08:54:18 by wiwu             ###   ########.fr       */
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

static int	is_it_directory(char *path)
{
	struct stat	st;

	stat(path, &st);
	if (S_ISDIR(st.st_mode))
		return (1);
	return (0);
}

static int	access_directory_problem(char *path)
{
	if (access(path, X_OK) != 0)
	{
		print_red("permissions denied");
		free(path);
		return (126);
	}
	if (is_it_directory(path))
	{
		print_2red("is a directory, not a cmd:", path);
		free(path);
		return (126);
	}
	return (1);
}

int	execute_simple_command(t_node *node, t_shell *shell)
{
	char		*path;

	if (!node->argv || !node->argv[0] || !node->argv[0][0])
		return (ft_putendl_fd("empty command", 2), 127);
	if (is_builtin(node->argv[0]))
	{
		if (node->redir)
			return (execute_builtin_redir(node->argv, node->redir, shell));
		return (execute_builtin(node->argv, shell));
	}
	if (node->argv[0][0] == '\0')
		return (0);
	path = get_path(node->argv[0], shell);
	if (!path)
	{
		ft_putstr_fd(node->argv[0], 2);
		ft_putendl_fd(": file / cmd / directory not found", 2);
		return (127);
	}
	if (access(path, X_OK) != 0)
		return (access_directory_problem(path));
	if (is_it_directory(path))
		return (access_directory_problem(path));
	return (exec_external_cmd(node, shell, path));
}
