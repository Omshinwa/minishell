/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/03 07:52:47 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/03 09:29:18 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clean_shell(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->env)
		clean_env(shell->env);
	free(shell);
}

void	clean_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		if (env->key)
			free(env->key);
		if (env->value)
			free(env->value);
		free(env);
		env = tmp;
	}
}

void	clean_nodes(t_node **node_addr)
{
	int		i;
	t_redir	*redir;
	t_redir	*redir_next;
	t_node	*node;

	if (!node_addr || !*node_addr)
		return ;
	node = *node_addr;
	clean_nodes(&node->left);
	clean_nodes(&node->right);
	i = 0;
	while (node->argv && node->argv[i])
		free(node->argv[i++]);
	free(node->argv);
	redir = node->redir;
	while (redir)
	{
		free(redir->filename);
		redir_next = redir->next;
		free(redir);
		redir = redir_next;
	}
	free(node);
	*node_addr = NULL;
}
