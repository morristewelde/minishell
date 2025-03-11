/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <sruzzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 10:05:46 by sruzzi            #+#    #+#             */
/*   Updated: 2023/11/08 10:20:26 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	str_len(const char *str)
{
	size_t	i;

	i = 0;
	while (*(str + i))
		i++;
	return (i);
}

char	*ft_strrchr(const char *s, int searched)
{
	char	*src;

	src = (char *)s + str_len(s);
	while (src >= s)
	{
		if (*src == (char)searched)
			return (src);
		src--;
	}
	return (NULL);
}
/*
int	main(void)
{
  char buf[SIZE] = "computer program";
  char * ptr;
  int    ch = 'p';
 
 // This illustrates strrchr
    ptr = ft_strrchr(buf, ch); // Modification : Utilisation de ft_strrchr
    if (ptr != NULL)
        printf("The last occurrence of %c in '%s' is '%c' 
        at position %ld\n", ch, 		buf, *ptr, ptr - buf);
    else
        printf("Character not found in the string.\n");
	return (0);
}*/
