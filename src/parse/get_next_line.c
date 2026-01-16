/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:37:09 by wiwu              #+#    #+#             */
/*   Updated: 2026/01/16 11:01:26 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>

// move the define to the header
#ifndef BUFFER_SIZE
# define BUFFER_SIZE 1
#endif

// remain is either NULL or has a \0 terminated string inside
static char	*get_next_line(int fd)
{
	static char	*remain;
	char		buffer[1];
	ssize_t		read_res;
	char		*temp;
	int len;

	remain = ft_strdup("");
	while (!ft_is_in_set(remain, '\n'))
	{
		len = ft_strlen(remain);
		read_res = read(fd, buffer, 1);
		if (read_res == -1)
		{
			free(remain);
			remain = NULL;
		}
		if (read_res <= 0)
			break ;
		temp = safe_malloc(len + 2);
		ft_memcpy(temp, remain, len);
		temp[len] = buffer[0];
		temp[len + 1] = '\0';
		free(remain);
		remain = temp;
	}
	return (remain);
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
