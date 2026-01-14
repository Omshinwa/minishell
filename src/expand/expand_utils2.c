/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 08:42:52 by wiwu              #+#    #+#             */
/*   Updated: 2026/01/14 17:14:44 by wiwu             ###   ########.fr       */
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

// TTY
// move every empty arg to the end of the array
// if all of them are empty, change argv[0] to 'true' (empty command)
// they even get moved past the NULL element
static void	shift_empty_args(char **argv)
{
	int	i;
	int	size;

	size = 0;
	while (argv[size] != NULL)
		size++;
	i = size;
	while (--i >= 0)
	{
		if (argv[i][0] == '\0')
		{
			free(argv[i]);
			shift_element(argv, i, size);
		}
	}
}

// first move the empty args to the end.
// If the first element is still NUll, that means
// all of them are Null, then we do a trick where we switch the
// first cmd with `true`
void	handle_empty_args(char ***argv)
{
	shift_empty_args(*argv);
	if (*argv && (*argv)[0] == NULL)
	{
		free_argv(*argv);
		(*argv) = safe_malloc(2 * sizeof(char *));
		(*argv)[0] = ft_strdup("true");
		(*argv)[1] = NULL;
	}
}
