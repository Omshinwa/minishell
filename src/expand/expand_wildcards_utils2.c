/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcards_utils2.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/10 06:57:29 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/10 07:09:50 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**expand_single_wildcard(char *arg)
{
	char	**matches;
	int		count;

	if (!has_wildcard(arg))
		return (NULL);
	count = count_matches(arg);
	if (count == 0)
		return (NULL);
	matches = malloc(sizeof(char *) * (count + 1));
	if (!matches)
		return (NULL);
	fill_matches(arg, matches);
	sort_matches(matches);
	return (matches);
}

int	count_matches_for_arg(char *arg)
{
	char	**matches;
	int		count;

	matches = expand_single_wildcard(arg);
	if (!matches)
		return (1);
	count = 0;
	while (matches[count])
		count++;
	free_argv(matches);
	return (count);
}

int	count_total_args(char **argv)
{
	int		i;
	int		count;

	i = 0;
	count = 0;
	while (argv[i])
	{
		count += count_matches_for_arg(argv[i]);
		i++;
	}
	return (count);
}
