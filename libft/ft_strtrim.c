/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <sruzzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 16:46:08 by sruzzi            #+#    #+#             */
/*   Updated: 2023/11/08 11:06:50 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isset(char c, const char *set)
{
	while (*set)
	{
		if (c == *set++)
			return (1);
	}
	return (0);
}

char	*ft_strtrim(const char *s1, const char *set)
{
	char	*ret;
	char	*start;
	char	*end;

	if (!s1 || !set)
		return (0);
	start = (char *)s1;
	end = start + ft_strlen(s1);
	while (*start && ft_isset(*start, set))
		++start;
	while (start < end && ft_isset(*(end - 1), set))
		--end;
	ret = ft_substr(start, 0, end - start);
	return (ret);
}
/*
int	main(void)
{
	const char	*input = "   Hello, World!   ";
	char	*trimmed = ft_strtrim(input, " ");
	if (trimmed == NULL)
	{
		printf("Erreur d'allocation de mémoire.\n");
		return (1);
    }
	printf("Chaîne originale : \"%s\"\n", input);
	printf("Chaîne trimmée : \"%s\"\n", trimmed);
	free(trimmed);
	return (0);
}*/
