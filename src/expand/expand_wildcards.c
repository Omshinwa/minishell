/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcards.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/10 06:55:06 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/13 16:54:50 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	add_matches(char ***new, int *j, char **matches)
{
	int	k;

	k = 0;
	while (matches[k])
	{
		(*new)[*j] = ft_strdup(matches[k]);
		if (!(*new)[*j])
			return (0);
		(*j)++;
		k++;
	}
	return (1);
}

static int	process_arg(char **argv, char **new_argv, int *j, int i)
{
	char	**matches;

	matches = expand_single_wildcard(argv[i]);
	if (matches)
	{
		if (!add_matches(&new_argv, j, matches))
		{
			free_argv(matches);
			return (0);
		}
		free_argv(matches);
	}
	else
	{
		new_argv[*j] = ft_strdup(argv[i]);
		if (!new_argv[*j])
			return (0);
		(*j)++;
	}
	return (1);
}

// FREES EVERY ELEMENT IN ARGV
char	**expand_wildcards_in_argv(char **argv)
{
	int		i;
	int		j;
	int		len;
	char	**new_argv;

	if (!argv)
		return (NULL);
	len = count_total_args(argv);
	new_argv = malloc(sizeof(char *) * (len + 1));
	if (!new_argv)
		return (NULL);
	i = 0;
	j = 0;
	while (argv[i])
	{
		if (!process_arg(argv, new_argv, &j, i))
		{
			free_argv(new_argv);
			return (NULL);
		}
		free(argv[i]);
		i++;
	}
	new_argv[j] = NULL;
	return (new_argv);
}
