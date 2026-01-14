/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 11:42:52 by wiwu              #+#    #+#             */
/*   Updated: 2026/01/14 17:55:33 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

// Returns an index, -1 if it didn't find.
// if c == '\0', equivalent to strlen
ssize_t	ft_strfind(const char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	if (c == '\0')
		return (i);
	return (-1);
}

char	*ft_strdup(const char *s)
{
	char	*res;
	int		i;
	int		len;

	len = ft_strfind(s, '\0');
	res = malloc(sizeof(char) * (len + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (i <= len)
	{
		res[i] = s[i];
		i++;
	}
	return (res);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*res;
	size_t			i;
	unsigned int	len_s;

	i = 0;
	len_s = ft_strfind(s, '\0');
	while (len_s > start && s[i + start] && i < len)
		i++;
	res = malloc(sizeof(char) * (i + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (len_s > start && s[i + start] && i < len)
	{
		res[i] = s[i + start];
		i++;
	}
	res[i] = 0;
	return (res);
}

// modified strjoin, it takes the len of s2 because s2 isn't always
// \0 terminated (it can be, only in the last case)
char	*ft_strjoin_len(char *s1, char *s2, size_t len_s2)
{
	char	*res;
	size_t	len_s1;
	size_t	i;

	if (s1 == NULL)
		len_s1 = 0;
	else
		len_s1 = ft_strfind(s1, '\0');
	res = malloc(sizeof(char) * (len_s1 + len_s2 + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (i < len_s1)
	{
		res[i] = s1[i];
		i++;
	}
	while (i < len_s1 + len_s2)
	{
		res[i] = s2[i - len_s1];
		i++;
	}
	res[i] = '\0';
	return (res);
}
