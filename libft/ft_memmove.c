/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <sruzzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 15:34:56 by sruzzi            #+#    #+#             */
/*   Updated: 2023/11/07 21:00:13 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t		i;
	const char	*s;
	char		*d;

	if (dst == NULL && src == NULL)
		return (NULL);
	s = (const char *)src;
	d = (char *)dst;
	i = 0;
	if (d > s)
	{
		while (len-- > 0)
			d[len] = s[len];
	}
	else
	{
		while (i < len)
		{
			d[i] = s[i];
			i++;
		}
	}
	return (dst);
}
/*
int main() {
    char src[] = "This is a test.";
    char dest[20];

    // Copie 5 caractères de src à dest, avec chevauchement
    ft_memmove(dest, src + 5, 5);

    printf("Destination : %s\n", dest);
    return 0;
}*/
