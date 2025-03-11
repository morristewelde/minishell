/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <sruzzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 15:22:28 by sruzzi            #+#    #+#             */
/*   Updated: 2023/11/07 19:27:23 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	char	*str;

	str = b;
	while (len > 0)
	{
		*str = (unsigned char) c;
		str++;
		len--;
	}
	return (b);
}
/*
int	main(void)
{
	// Test avec une chaine de caractère
	char str[] = "almost every programmer should know memset!";
	memset (str,'-',  6 * sizeof(str[0]));
	puts (str);

	// Test avec un tableau de int
	int t[] = {145478, 1458758, 1421454, 14254585};
	memset(t, 0, 3*sizeof(t[0]));

	int i = 0;
	for(i = 0; i < 4; i++)
	{
		printf("%d\n", t[i]);
	}
	return (0);
}*/
