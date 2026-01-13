/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 08:42:52 by wiwu              #+#    #+#             */
/*   Updated: 2026/01/13 11:18:29 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// I: index of element to move
// END: target element to move to
static void	shift_element(char **argv, int i, int end)
{
	int		j;
	char	*tmp;

	j = i - 1;
	while (++j < end)
	{
		tmp = argv[j];
		argv[j] = argv[j + 1];
		argv[j + 1] = tmp;
	}
}

// move every empty arg to the end of the array
// if all of them are empty, change argv[0] to 'true' (empty command)
// if theres exist a NULL arg and we encountered a non NULL arg,
// move the NULL arg to the end
void	shift_all_args(char **argv)
{
	int	i;
	int	size;
	int	exist_non_null;

	size = 0;
	while (argv[size] != NULL)
		size++;
	exist_non_null = 0;
	i = size;
	while (--i >= 0)
	{
		if (argv[i][0] == '\0')
		{
			free(argv[i]);
			shift_element(argv, i, size);
		}
		else
			exist_non_null = 1;
	}
	if (exist_non_null == 0)
	{
		argv[0] = ft_strdup("true");
		argv[1] = NULL;
	}
}
