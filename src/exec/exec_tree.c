/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 11:39:43 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/13 18:37:47 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_signal.h"

int	execute_subshell(t_node *node, t_shell *shell)
{
	pid_t	pid;
	int		status;
	int		code;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
	{
		code = execute_tree(node->left, shell);
		clean_shell(shell);
		exit(code);
	}
	ignore_signals();
	waitpid(pid, &status, 0);
	return (get_exit_status(status));
}

static int	execute_and(t_node *node, t_shell *shell)
{
	int	status;

	status = execute_tree(node->left, shell);
	if (status == 0)
		return (execute_tree(node->right, shell));
	return (status);
}

static int	execute_or(t_node *node, t_shell *shell)
{
	int	status;

	status = execute_tree(node->left, shell);
	if (status != 0)
		return (execute_tree(node->right, shell));
	return (status);
}

int	execute_tree(t_node *node, t_shell *shell)
{
	if (!node)
		return (0);
	if (node->type == NODE_AND)
		return (execute_and(node, shell));
	if (node->type == NODE_OR)
		return (execute_or(node, shell));
	if (node->type == NODE_PIPE)
		return (execute_pipeline(node, shell));
	if (node->type == NODE_SUBSHELL)
		return (execute_subshell(node, shell));
	if (node->type == NODE_CMD)
		return (execute_simple_command(node, shell));
	return (0);
}
