/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 07:40:33 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/12 21:54:41 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_dollar(char *s, int *i, char **result, t_shell *shell)
{
	char	*var_name;
	char	*var_value;
	int		name_len;

	(*i)++;
	name_len = 0;
	var_name = extract_var_name(s, *i);
	if (!var_name)
		return (0);
	var_value = get_var_value(shell, var_name);
	*result = append_to_result(*result, var_value);
	if (ft_strcmp(var_name, "?") == 0)
		name_len = 1;
	else
		name_len += ft_strlen(var_name);
	*i += name_len;
	free(var_name);
	free(var_value);
	return (1);
}

static void	handle_normal_char(char *s, int *i, char **result)
{
	char	temp[2];

	temp[0] = s[*i];
	temp[1] = '\0';
	*result = append_to_result(*result, temp);
	(*i)++;
}

char	*remove_quotes(char *s, t_shell *shell)
{
	int		i;
	int		in_single;
	int		in_double;
	char	*result;

	(void) shell;
	i = 0;
	in_single = 0;
	in_double = 0;
	result = ft_strdup("");
	while (s[i])
	{
		update_quote_state(s[i], &in_single, &in_double);
		if ((s[i] == '\'' && !in_double) || (s[i] == '"' && !in_single))
			i++;
		else
			handle_normal_char(s, &i, &result);
	}
	return (result);
}

static int	should_expand(char *s, int i, int ignore_quotes, int in_single)
{
	if (s[i] != '$' || s[i + 1] == '\0')
		return (0);
	if (ignore_quotes)
		return (1);
	if (!in_single && (is_var_char(s[i + 1]) || s[i + 1] == '?'))
		return (1);
	return (0);
}

char	*expand_variable(char *s, t_shell *shell, int ignore_quotes)
{
	int		i;
	int		in_single;
	int		in_double;
	char	*result;

	i = 0;
	in_single = 0;
	in_double = 0;
	result = ft_strdup("");
	while (s[i])
	{
		update_quote_state(s[i], &in_single, &in_double);
		if (should_expand(s, i, ignore_quotes, in_single))
		{
			if (!handle_dollar(s, &i, &result, shell))
				return (free(result), NULL);
		}
		else
			handle_normal_char(s, &i, &result);
	}
	return (result);
}
