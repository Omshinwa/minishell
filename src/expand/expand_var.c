/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 07:40:33 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/11 20:41:45 by wiwu             ###   ########.fr       */
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

// Remove quotes and expand $VARIABLES in the string S
// $VARIABLES are found in shell->env
char	*expand_strings(char *s, t_shell *shell)
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
		if ((s[i] == '\'' && !in_double) || (s[i] == '"' && !in_single))
			i++;
		else if (s[i] == '$' && !in_single && s[i + 1])
		{
			if (!handle_dollar(s, &i, &result, shell))
				return (free(result), NULL);
		}
		else
			handle_normal_char(s, &i, &result);
	}
	return (result);
}

// Remove quotes and expand $VARIABLES in the string S
// $VARIABLES are found in shell->env
// if EXPAND_DOLLAR is 0, it doesnt expand variables

// : remove quotes
// : expand variables if in single quote (default)
// : expand variables even if not in single quote
// : dont expand
// : expand when there's single quote
// removed && (!in_single || expand_all) 
char	*expand_strings_options(char *s, t_shell *shell, int do_rmv_quotes, int do_exp_vars)
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
		if (do_rmv_quotes && ((s[i] == '\'' && !in_double) || (s[i] == '"' && !in_single)))
    		i++;
		else if (do_exp_vars && s[i] == '$' && s[i + 1])
		{
			if (!handle_dollar(s, &i, &result, shell))
				return (free(result), NULL);
		}
		else
			handle_normal_char(s, &i, &result);
	}
	return (result);
}
