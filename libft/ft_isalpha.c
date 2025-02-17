/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <sruzzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 15:13:44 by sruzzi            #+#    #+#             */
/*   Updated: 2023/11/07 19:22:04 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalpha(int arg)
{
	if (arg < 'A' || (arg > 'Z' && arg < 'a') || arg > 'z')
	{
		return (0);
	}
	else
		return (1);
}

/*int	main(void)
{
	char	c;

	c = 'Q';
	printf("\nResult when uppercase alphabet is passed: %d", ft_isalpha(c));
	c = 'q';
	printf("\nResult when lowercase alphabet is passed: %d", ft_isalpha(c));
	c = '+';
	printf("\nResult when non-alphabetic char is passed: %d", ft_isalpha(c));
	return (0);
}*/