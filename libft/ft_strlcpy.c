/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 14:18:25 by dasamuel          #+#    #+#             */
/*   Updated: 2025/11/13 07:51:52 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	lensrc;

	lensrc = ft_strlen(src);
	if (dstsize == 0)
		return (lensrc);
	while (*src && dstsize-- > 1)
		*dst++ = *src++;
	*dst = '\0';
	return (lensrc);
}
