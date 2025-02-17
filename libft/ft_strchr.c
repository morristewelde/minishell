/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <sruzzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 10:56:56 by sruzzi            #+#    #+#             */
/*   Updated: 2023/11/07 19:50:57 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(char const *str, int c)
{
	while (*str)
	{
		if (*str == (char)c)
		{
			return ((char *)str);
		}
		str++;
	}
	if (*str == (char)c)
		return ((char *)str);
	else
		return (NULL);
}
/*
int	main(void)
{
    const char *str = "Hello, World!";
    int charToFind = 'W';

    const char *result = ft_strchr(str, charToFind);

    if (result) {
        printf("Le caractère '%c' a été trouvé à la position 
        : %ld\n", charToFind, result - str);
    } else {
        printf("Le caractère '%c' n'a pas été
         trouvé dans la chaîne.\n", charToFind);
    }

    return 0;
}*/
