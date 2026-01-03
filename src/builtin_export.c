/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 13:17:09 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/02 12:52:59 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_export(char **args, t_shell *shell)
{
	int		i;
	char	*key;
	char	*value;
	t_env	*current;

	current = shell->env;
	if (!args[1])
	{
		while (current)
		{
			printf("declare -x %s=%s\n", current->key, current->value);
			current = current->next;
		}
		return (0);
	}
	i = 0;
	while (args[++i])
	{
		key = extract_key(args[i]);
		value = extract_value(args[i]);
		set_env_value(shell, key, value);
		free(key);
		free(value);
	}
	return (0);
}
