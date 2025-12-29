/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 20:41:31 by dasamuel          #+#    #+#             */
/*   Updated: 2025/11/13 10:25:54 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_numlen(long n)
{
	size_t	len;

	len = 0;
	if (n == 0)
		return (1);
	if (n < 0)
	{
		len++;
		n = -n;
	}
	if (n == 0)
		len++;
	while (n > 0)
	{
		len++;
		n /= 10;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	size_t	len;
	long	nb;
	char	*nbr;

	nb = n;
	len = ft_numlen(nb);
	nbr = malloc(len + 1);
	if (!nbr)
		return (NULL);
	nbr[len] = '\0';
	if (nb == 0)
		nbr[0] = '0';
	if (nb < 0)
	{
		nbr[0] = '-';
		nb = -nb;
	}
	while (nb > 0)
	{
		nbr[--len] = nb % 10 + '0';
		nb /= 10;
	}
	return (nbr);
}
