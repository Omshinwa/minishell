/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 13:43:56 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/09 06:50:04 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_left_pipe(t_node *node, t_shell *shell, int pipefd[2])
{
	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	exit(execute_tree(node->left, shell));
}

static void	exec_right_pipe(t_node *node, t_shell *shell, int pipefd[2])
{
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	exit(execute_tree(node->right, shell));
}

static int	wait_for_children(pid_t pid_left, pid_t pid_right)
{
	int	status_left;
	int	status_right;
	int	exit_status;

	exit_status = 0;
	waitpid(pid_left, &status_left, 0);
	waitpid(pid_right, &status_right, 0);
	exit_status = get_exit_status(status_right);
	return (exit_status);
}

static int	handle_pipe_error(int pipefd[2], pid_t pid_left)
{
	perror("fork");
	close(pipefd[0]);
	close(pipefd[1]);
	if (pid_left > 0)
	{
		kill(pid_left, SIGTERM);
		waitpid(pid_left, NULL, 0);
	}
	return (1);
}

int	execute_pipeline(t_node *node, t_shell *shell)
{
	int		pipefd[2];
	pid_t	pid_left;
	pid_t	pid_right;

	if (pipe(pipefd) == -1)
		return (perror("pipe"), 1);
	pid_left = fork();
	if (pid_left == -1)
		return (handle_pipe_error(pipefd, 0));
	if (pid_left == 0)
		exec_left_pipe(node, shell, pipefd);
	pid_right = fork();
	if (pid_right == -1)
		return (handle_pipe_error(pipefd, pid_left));
	if (pid_right == 0)
		exec_right_pipe(node, shell, pipefd);
	close(pipefd[0]);
	close(pipefd[1]);
	return (wait_for_children(pid_left, pid_right));
}
