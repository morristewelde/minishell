/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <sruzzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 17:22:58 by sruzzi            #+#    #+#             */
/*   Updated: 2023/11/07 19:23:32 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isprint(int c)
{
	if (c >= 32 && c <= 126)
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
	printf("\nResult when uppercase alphabet is passed: %d", ft_isprint(c));
	c = 'q';
	printf("\nResult when lowercase alphabet is passed: %d", ft_isprint(c));
	c = '+';
	printf("\nResult when non-alphabetic char is passed: %d", ft_isprint(c));
	c = 32;
	printf("\nResult when space is passed: %d", ft_isprint(c));
	c = '2';
	printf("\nResult when a digital char is passed: %d", ft_isprint(c));
	c = '?';
	printf("\nResult when a punctuation mark is passed: %d", ft_isprint(c));
	c = 8;
	printf("\nResult when backspace is passed: %d", ft_isprint(c));
	return (0);
}*/