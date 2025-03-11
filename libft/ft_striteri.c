/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <sruzzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 11:02:40 by sruzzi            #+#    #+#             */
/*   Updated: 2023/11/07 19:53:05 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	size_t	i;

	if (s == (char *) NULL || (void *)f == NULL)
		return ;
	i = 0;
	while (s[i] != '\0')
	{
		f((unsigned int)i, &s[i]);
		i++;
	}
}
/*
void print_index_and_char(unsigned int index, char *character) {
    printf("Caractère à l'indice %zu : %c\n", index, *character);
}

int main() {
    char maChaine[] = "Hello, World!";
    
    // Appeler ft_striteri avec la fonction print_index_and_char
    ft_striteri(maChaine, print_index_and_char);

    return (0);
}*/
