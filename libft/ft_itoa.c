/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <sruzzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 16:40:42 by sruzzi            #+#    #+#             */
/*   Updated: 2023/11/07 20:38:09 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	nb_lenn(long n)
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
	i = nb_lenn((long) n);
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
/*
int	main()
{
    int number = -12345;
    char *result = ft_itoa(number);

    if (result) {
        printf("Integer to String: %s\n", result);
        free(result); // N'oubliez pas de libérer la mémoire allouée
    } else {
        printf("Memory allocation failed.\n");
    }

    return (0);
}*/
