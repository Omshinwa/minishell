/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 08:07:43 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/02 08:08:27 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*new_env_node(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	node->value = ft_strdup(value);
	node->next = NULL;
	return (node);
}

void	add_env_node(t_env **env, t_env *new)
{
	t_env	*current;

	if (!*env)
	{
		*env = new;
		return ;
	}
	current = *env;
	while (current->next)
		current = current->next;
	current->next = new;
}

char	*extract_key(char *s)
{
	int		i;
	int		len;
	char	*key;

	if (!ft_is_in_set(s, '='))
		return (NULL);
	i = 0;
	len = ft_strnlen(s, '=');
	key = malloc(len + 1);
	if (!key)
		return (NULL);
	while (i < len)
	{
		key[i] = s[i];
		i++;
	}
	key[i] = '\0';
	return (key);
}

char	*extract_value(char *s)
{
	int		i;
	int		len;
	int		start;
	char	*value;

	if (!ft_is_in_set(s, '='))
		return (NULL);
	i = 0;
	start = ft_strnlen(s, '=');
	len = ft_strlen(s) - start - 1;
	value = malloc(len + 1);
	if (!value)
		return (NULL);
	start++;
	while (i < len)
	{
		value[i] = s[start + i];
		i++;
	}
	value[i] = '\0';
	return (value);
}

t_env	*init_env(char **envp)
{
	int		i;
	t_env	*env;
	char	*key;
	char	*value;

	i = 0;
	env = NULL;
	while (envp[i])
	{
		key = extract_key(envp[i]);
		value = extract_value(envp[i]);
		if (key && value)
		{
			add_env_node(&env, new_env_node(key, value));
			free(key);
			free(value);
		}
		i++;
	}
	return (env);
}
