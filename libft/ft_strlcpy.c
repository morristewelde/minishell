/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <sruzzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 10:33:52 by sruzzi            #+#    #+#             */
/*   Updated: 2023/11/07 20:20:30 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	src_len;

	if (!dst || !src)
		return (0);
	src_len = ft_strlen(src);
	if (size == 0)
		return (src_len);
	i = 0;
	while (src[i] && i < size - 1)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (src_len);
}
/*
int	main(void)
{
	char	src[100] = "Bonjour le monde";	
	char	dst[100] = "";
	size_t	size;

	size = 16;
	ft_strlcpy(dst, src, size);
	printf ("Source: %s\n", src);
	printf ("Destination: %s\n", dst);
	printf ("Longueur de la source: %lu\n", ft_strlcpy(dst, src, size));
	return (0);
}*/
