/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <sruzzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 16:30:41 by sruzzi            #+#    #+#             */
/*   Updated: 2023/11/07 19:51:33 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strdup(const char *src)
{
	char	*new;
	int		i;
	int		size;

	size = 0;
	while (src[size])
		++size;
	new = malloc(size + 1);
	if (!new)
		return (NULL);
	i = 0;
	while (src[i])
	{
		new[i] = src[i];
		i++;
	}
	new[i] = '\0';
	return (new);
}
/*
int main()
{
    const char *source = "Hello, World!";
    char *duplicate = ft_strdup(source);

    if (duplicate == NULL)
    {
        printf("Erreur d'allocation de mémoire.\n");
        return 1;
    }

    printf("Chaîne source : %s\n", source);
    printf("Chaîne dupliquée : %s\n", duplicate);

    // N'oubliez pas de libérer la mémoire allouée dynamiquement
    free(duplicate);

    return 0;
}*/
