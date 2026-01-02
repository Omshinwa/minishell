/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 08:58:39 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/02 09:27:45 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*find_env_node(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

void	set_env_value(t_shell *shell, char *key, char *value)
{
	t_env	*node;
	t_env	*last;

	node = find_env_node(shell->env, key);
	if (node)
	{
		free(node->value);
		node->value = ft_strdup(value);
		return ;
	}
	if (!shell->env)
		shell->env = new_env_node(key, value);
	else
	{
		last = shell->env;
		while (last->next)
			last = last->next;
		last->next = new_env_node(key, value);
	}
}
