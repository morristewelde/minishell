/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <sruzzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 10:50:19 by sruzzi            #+#    #+#             */
/*   Updated: 2023/11/07 20:52:58 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*strcpy(char *dst, const char *src)
{
	while (*src)
	{
		*dst = *src;
		src++;
		dst++;
	}
	*dst = '\0';
	return (dst);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len1;
	size_t	len2;
	char	*result;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	result = (char *)malloc (len1 + len2 +1);
	if (result == NULL)
		return (NULL);
	strcpy(result, s1);
	strcpy(result + len1, s2);
	return (result);
}
/*
int	main(void)
{
	const char *str1 = "Hello, ";
	const char *str2 = "World!";
	char *joined_str;

	joined_str = ft_strjoin(str1, str2);
	if (joined_str)
	{
		printf("Joined String: %s\n", joined_str);
		free(joined_str);
      } 
	else
	{
		printf("Memory allocation failed.\n");
	}
	return (0);
}*/
