/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <sruzzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 10:00:10 by sruzzi            #+#    #+#             */
/*   Updated: 2023/11/07 19:26:24 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	while (n--)
	{
		if (*(unsigned char *)s1 != *(unsigned char *)s2)
			return (*(unsigned char *)s1 - *(unsigned char *)s2);
		s1 = (unsigned char *)s1 + 1;
		s2 = (unsigned char *)s2 + 1;
	}
	return (0);
}
/*
int	main(void)
{
	const char	*str1 = "Hello, World!";
	const char	*str2 = "Hello,Big World!";
	size_t	n = 13; // You can change this value to test different lengths

	int	result = ft_memcmp(str1, str2, n);

	if (result < 0)
	{
		printf("str1 is less than str2\n");
	}
	else if (result > 0)
	{
		printf("str1 is greater than str2\n");
	}
	else
	{
		printf("str1 and str2 are equal\n");
	}
    return 0;
}*/
