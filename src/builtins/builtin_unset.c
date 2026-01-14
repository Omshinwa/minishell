/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 13:16:50 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/14 07:40:17 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	unset_env_value(t_shell *shell, char *key)
{
	t_env	*current;
	t_env	*prev;
	t_env	*temp;

	current = shell->env;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			temp = current;
			if (prev)
				prev->next = current->next;
			else
				shell->env = current->next;
			current = current->next;
			free(temp->key);
			free(temp->value);
			free(temp);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

int	ft_unset(char **args, t_shell *shell)
{
	int	i;

	i = 1;
	while (args[i])
	{
		unset_env_value(shell, args[i]);
		i++;
	}
	return (0);
}
