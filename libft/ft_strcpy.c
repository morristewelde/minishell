/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <sruzzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 11:17:29 by sruzzi            #+#    #+#             */
/*   Updated: 2023/11/07 20:23:28 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strcpy(char *dst, const char *src)
{
	char	*original_dst;

	original_dst = dst;
	while (*src)
	{
		*dst = *src;
		src++;
		dst++;
	}
	*dst = '\0';
	return (original_dst);
}
/*
int	main(void)
{
	char	destination[20];
	const char *source = "Hello, World !";
	ft_strcpy(destination, source);
	printf("Copied String on destination: %s\n", destination);
	return (0);
}*/
