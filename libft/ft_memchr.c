/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <sruzzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 15:19:19 by sruzzi            #+#    #+#             */
/*   Updated: 2023/11/07 19:26:08 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(void *s, int c, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		if (((unsigned char *) s)[i] == (unsigned char)c)
		{
			return (s + i);
		}
		i++;
	}
	return (NULL);
}
/*int main() {
    char str[] = "Bonjour, le monde!";
    char character = 'l';
    size_t str_len = sizeof(str) - 1; //-1 pour exclure le caractère nul

    // Utilisation de la fonction ft_memchr pour rechercher
     le caractère dans la chaîne
    void *result = ft_memchr(str, character, str_len);

    if (result != NULL) {
        printf("Le caractère '%c' a été trouvé à la position %ld 
        dans la chaîne.\n", character, (char *)result - str);
    } else {
        printf("Le caractère '%c' n'a pas été trouvé dans la 
        chaîne.\n", character);
    }

    return 0;
}*/
