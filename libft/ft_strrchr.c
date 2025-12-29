/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 12:46:34 by dasamuel          #+#    #+#             */
/*   Updated: 2025/11/14 10:55:11 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t	len;

	len = ft_strlen(s);
	if (s[len] == (char)c)
		return ((char *)&s[len]);
	while (len--)
	{
		if (s[len] == (char)c)
			return ((char *)&s[len]);
	}
	return (NULL);
}
