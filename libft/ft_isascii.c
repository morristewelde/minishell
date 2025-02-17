/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <sruzzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 16:58:14 by sruzzi            #+#    #+#             */
/*   Updated: 2023/11/07 19:22:47 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isascii(int c)
{
	if (c >= 0 && c <= 127)
	{
		return (1);
	}
	else
		return (0);
}

/*
int	main(void)
{
	char	c;

	c = 'Q';
	printf("\nResult when uppercase alphabet is passed: %d", ft_isascii(c));
	c = 'q';
	printf("\nResult when lowercase alphabet is passed: %d", ft_isascii(c));
	c = '+';
	printf("\nResult when non-alphabetic char is passed: %d", ft_isascii(c));
	c = '2';
	printf("\nResult when a digital char is passed: %d", ft_isascii(c));
	return (0);
}*/