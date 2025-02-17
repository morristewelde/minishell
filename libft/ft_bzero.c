/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <sruzzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 11:50:09 by sruzzi            #+#    #+#             */
/*   Updated: 2023/11/07 19:20:20 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	char	*str;

	str = s;
	while (n > 0)
	{
		*str = 0;
		str++;
		n--;
	}
}
/*
int	main(void)
{
	char	str[10] = "Hello";
	printf("Avant la fonction ft_bzero: %s\n", str);
	ft_bzero(str, 10);
	printf("Apres la fonction ft_bzero: %s\n", str);
	return (0);
}*/
