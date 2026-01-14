/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:37:09 by wiwu              #+#    #+#             */
/*   Updated: 2026/01/14 20:35:23 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "get_next_line.h"
#include <errno.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 10
#endif

// Split into two part. Return the first part, save the second part to `remain`
static char	*trim_remain(char **remain)
{
	ssize_t	nline_pos;
	char	*first_part;
	char	*second_part;
	char	*temp;

	if (*remain == NULL)
		return (NULL);
	nline_pos = ft_strfind(*remain, '\n');
	if (nline_pos == -1)
		return (temp = ft_strdup(*remain), free(*remain), *remain = NULL, temp);
	first_part = ft_substr(*remain, 0, nline_pos + 1);
	if (!first_part)
		return (free(*remain), *remain = NULL, NULL);
	second_part = ft_substr(*remain, nline_pos + 1, ft_strfind(*remain, '\0'));
	if (!second_part)
		return (free(first_part), free(*remain), *remain = NULL, NULL);
	if (*second_part == '\0')
	{
		free(second_part);
		second_part = NULL;
	}
	free(*remain);
	*remain = second_part;
	return (first_part);
}

static int	newline_in(char *remain)
{
	if (remain == NULL)
		return (0);
	if (ft_strfind(remain, '\n') != -1)
		return (1);
	return (0);
}

// remain is either NULL or has a \0 terminated string inside
static char	*get_next_line(int fd)
{
	static char	*remain;
	char		*buffer;
	ssize_t		read_res;
	char		*temp;

	buffer = malloc(sizeof(char) * BUFFER_SIZE);
	if (!buffer)
		return (NULL);
	while (!newline_in(remain))
	{
		read_res = read(fd, buffer, BUFFER_SIZE);
		if (read_res == -1)
		{
			free(remain);
			remain = NULL;
		}
		if (read_res <= 0)
			break ;
		temp = ft_strjoin_len(remain, buffer, read_res);
		free(remain);
		remain = temp;
		if (temp == NULL)
			break ;
	}
	return (free(buffer), trim_remain(&remain));
}

char	*get_next_line_strip_nl(int fd)
{
	char	*tmp;
	char	*line;

	tmp = get_next_line(fd);
	if (errno == EINTR)
	{
		free(tmp);
		return (NULL);
	}
	line = ft_strtrim(tmp, "\n");
	free(tmp);
	return (line);
}