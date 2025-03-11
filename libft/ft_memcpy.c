/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <sruzzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 17:33:00 by sruzzi            #+#    #+#             */
/*   Updated: 2023/11/07 20:55:47 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t		i;
	const char	*src_char;
	char		*dest_char;

	if (!dest && !src)
		return (NULL);
	dest_char = (char *)dest;
	src_char = (const char *)src;
	i = 0;
	while (i < n)
	{
		dest_char[i] = src_char[i];
		i++;
	}
	return (dest);
}
/*
void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;
	char	*s1;

	if (dest == NULL && src == NULL)
		return (NULL);
	s1 = (char *) dest;
	i = 0;
	while (i < n)
	{
		*(char *) s1 = *(char *) src;
		s1++;
		src++;
		i++;
	}
	return (dest);
}

int main()
{
    char src[10] = "Hello";
    char dst[10];
    printf("Avant ft_memcpy: %s\n", dst);
    ft_memcpy(dst, src, 10);
    printf("Après ft_memcpy: %s\n", dst);
    return 0;
}*/
