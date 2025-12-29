/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 17:51:12 by dasamuel          #+#    #+#             */
/*   Updated: 2025/11/13 08:30:54 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	total;
	void	*s;

	if (size != 0 && nmemb > (SIZE_MAX / size))
		return (NULL);
	total = nmemb * size;
	s = malloc(total);
	if (!s)
		return (NULL);
	ft_bzero(s, total);
	return (s);
}
