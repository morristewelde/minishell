/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <sruzzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 15:38:44 by sruzzi            #+#    #+#             */
/*   Updated: 2023/11/07 20:00:43 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	if (n == 0)
		return (0);
	while (*s1 && *s2 && *s1 == *s2 && n > 1)
	{
		s1++;
		s2++;
		n--;
	}
	return (((unsigned char)(*s1) - (unsigned char)(*s2)));
}
/*
int	main(void)
{
	size_t	n;
	const char	*s1;
	const char	*s2;
	int	result;

	s1 = "Bonjour le monde";
	s2 = "Bonjour les gens";
	n = 15;
	result = ft_strncmp(s1, s2, n);
	printf("Resultat de la comparaison : %d\n", result);
	return (0);
}*/
