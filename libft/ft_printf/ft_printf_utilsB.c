/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*     ft_printf_utilsB.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 15:09:15 by sruzzi            #+#    #+#             */
/*   Updated: 2024/01/08 15:13:54 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

size_t	nb_len(long n)
{
	size_t	i;

	i = 0;
	if (n < 0)
	{
		i = 1;
		n = -n;
	}
	if (n == 0)
		i = 1;
	else
	{
		while (n > 0)
		{
			n = n / 10;
			i++;
		}
	}
	return (i);
}

char	*ft_itoa(int n)
{
	char	*res;
	size_t	i;
	long	nb;
	int		neg;

	neg = 0;
	i = nb_len((long) n);
	nb = (long) n;
	res = (char *)malloc(sizeof(char) * (i + 1));
	if (!res)
		return (NULL);
	if (nb < 0)
	{
		nb = -nb;
		res[0] = '-';
		neg = 1;
	}
	res[i] = '\0';
	while (i > (size_t) neg)
	{
		res[i - 1] = nb % 10 + '0';
		nb = nb / 10;
		i--;
	}
	return (res);
}
