/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 13:17:09 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/15 07:40:38 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_identifier(char *s)
{
	int	i;

	if (!s || !s[0])
		return (0);
	if (!(ft_isalpha(s[0]) || s[0] == '_'))
		return (0);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!(ft_isalnum(s[i]) || s[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

void	print_export_error(char *arg)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd("': not a valid identifier", 2);
}

static int	handle_key_no_equal(char *s, t_shell *shell)
{
	int		i;
	int		len;
	char	*key;

	len = 0;
	while (s[len] && s[len] != '=')
		len++;
	key = malloc(len + 1);
	if (!key)
		return (1);
	i = 0;
	while (i < len)
	{
		key[i] = s[i];
		i++;
	}
	key[i] = '\0';
	if (!find_env_node(shell->env, key))
		set_env_value(shell, key, "");
	free(key);
	return (0);
}

static int	handle_export_arg(char *arg, t_shell *shell)
{
	char	*key;
	char	*value;

	if (!is_valid_identifier(arg))
	{
		print_export_error(arg);
		return (1);
	}
	if (!ft_is_in_set(arg, '='))
		return (handle_key_no_equal(arg, shell));
	key = extract_key(arg);
	value = extract_value(arg);
	if (!key)
	{
		if (value)
			free(value);
		return (1);
	}
	if (!value)
		value = ft_strdup("");
	set_env_value(shell, key, value);
	free(key);
	free(value);
	return (0);
}

int	ft_export(char **args, t_shell *shell)
{
	int		i;
	int		exit;
	t_env	*current;

	exit = 0;
	current = shell->env;
	if (!args[1])
	{
		while (current)
		{
			if (ft_strcmp(current->value, "") != 0)
				printf("declare -x %s=\"%s\"\n", current->key, current->value);
			else
				printf("declare -x %s\n", current->key);
			current = current->next;
		}
		return (0);
	}
	i = 0;
	while (args[++i])
	{
		if (handle_export_arg(args[i], shell))
			exit = 1;
	}
	return (exit);
}
