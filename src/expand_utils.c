/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 07:03:09 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/05 09:22:27 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

char	*extract_var_name(char *s, int i)
{
	int		start;
	int		len;
	char	*var_name;

	len = 0;
	start = i;
	if (s[i] == '?')
		return (ft_strdup(""));
	while (s[i] && is_var_char(s[i]))
	{
		i++;
		len++;
	}
	var_name = malloc(len + 1);
	if (!var_name)
		return (NULL);
	ft_strlcpy(var_name, s + start, len + 1);
	return (var_name);
}

char	*get_var_value(t_shell *shell, char *var_name)
{
	char	*value;

	if (ft_strcmp(var_name, "?") == 0)
		return (ft_itoa(shell->last_exit));
	value = get_env_value(shell, var_name);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}

char	*append_to_result(char *result, char *to_add)
{
	char	*new_result;

	if (!result)
		return (ft_strdup(to_add));
	new_result = ft_strjoin(result, to_add);
	free(result);
	return (new_result);
}

void	update_quote_state(char c, int *in_single, int *in_double)
{
	if (c == '\'' && !(*in_double))
		*in_single = !(*in_single);
	if (c == '"' && !(*in_single))
		*in_double = !(*in_double);
}
