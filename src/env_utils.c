/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 08:10:12 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/03 08:43:15 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_env(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

char	**env_to_array(t_shell *shell)
{
	int		i;
	int		count;
	char	*temp;
	char	**envp;
	t_env	*tmp;

	i = 0;
	tmp = shell->env;
	count = count_env(shell->env);
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	while (i < count)
	{
		temp = ft_strjoin(tmp->key, "=");
		envp[i++] = ft_strjoin(temp, tmp->value);
		tmp = tmp->next;
		free(temp);
	}
	envp[i] = NULL;
	return (envp);
}

char	*get_env_value(t_shell *shell, char *key)
{
	t_env	*current;

	current = shell->env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}
