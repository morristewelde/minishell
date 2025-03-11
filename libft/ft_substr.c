/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <sruzzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 15:21:03 by sruzzi            #+#    #+#             */
/*   Updated: 2023/11/08 10:54:30 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub_str;
	char	*new_str;
	size_t	s_len;
	size_t	i;

	s_len = ft_strlen(s);
	if (s == NULL || s_len < start)
		return (ft_strdup(""));
	if (start + len < s_len)
		sub_str = (char *)malloc((len + 1) * sizeof(char));
	else
		sub_str = (char *)malloc((s_len - start + 1) * sizeof(char));
	if (sub_str == NULL)
		return (NULL);
	i = start;
	new_str = sub_str;
	while (i < (start + len) && *(s + i))
		*new_str++ = *(s + i++);
	*new_str = '\0';
	return (sub_str);
}
/*
int	main(void)
{
	const char *source ="Ceci est un exemple de sous-chaîne.";
	unsigned int start = 8;  
	size_t len = 11;         	
	char *sub_string = ft_substr(source, start, len);
	if (sub_string == NULL)
	{
		printf("Erreur d'allocation de mémoire.\n");
		return (1);
	}

    printf("Chaîne source : %s\n", source);
    printf("Sous-chaîne : %s\n", sub_string);

    free(sub_string);
    return (0);
}*/
