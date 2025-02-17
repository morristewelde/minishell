/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <sruzzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 17:41:21 by sruzzi            #+#    #+#             */
/*   Updated: 2023/11/08 09:31:06 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t ecount, size_t esize)
{
	size_t	total;
	char	*mem;
	size_t	i;

	total = ecount * esize;
	mem = malloc(total);
	if (mem == NULL)
		return (NULL);
	i = 0;
	while (i < total)
	{
		mem[i] = 0;
		i++;
	}
	return (mem);
}

/*int	main(void)
{
    // Utilisation de ft_calloc
    int *arr = (int *)ft_calloc(5, sizeof(int));

    if (arr == NULL)
    {
        printf("Erreur d'allocation de mémoire.\n");
        return 1;
    }

    // Initialisation des éléments du tableau
    for (int i = 0; i < 5; i++)
    {
        arr[i] = i;
    }

    // Affichage des éléments du tableau
    printf("Contenu du tableau :\n");
    for (int i = 0; i < 5; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // Libération de la mémoire allouée
    free(arr);

    return 0;
}*/