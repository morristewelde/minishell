/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <sruzzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 15:34:16 by sruzzi            #+#    #+#             */
/*   Updated: 2023/11/07 20:20:47 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t n)
{
	size_t	i;
	size_t	ls;
	size_t	td;

	i = 0;
	td = 0;
	ls = ft_strlen(src);
	while (dest[td] != '\0' && td < n)
		td++;
	if (n == 0)
		return (ls);
	if (n <= td)
		return (ls + n);
	while (i < (n - 1 - td) && (src[i] != '\0'))
	{
		dest[td + i] = src[i];
		i++;
	}
	dest[td + i] = '\0';
	return (td + ls);
}
/*
int	main(void)
{
	char	dest[150] ="Ceci est un test de la fonction strlen";
	char	src[] =" pour les 42Ziens de 42!!";
	printf("%s\n", dest);
	size_t	result;
	result = ft_strlcat(dest, src, 43);
	printf("%s\n qui nous retourne %ld", dest, result);
	return (0);  
}*/
