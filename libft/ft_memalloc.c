/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <sruzzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 18:43:50 by sruzzi            #+#    #+#             */
/*   Updated: 2023/11/07 20:21:59 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memalloc(size_t size)
{
	unsigned char	*ptr;

	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	ft_memset(ptr, 0, size);
	return (ptr);
}

/*int	main(void)
{
    size_t size = 10;
    char *ptr = (char *)ft_memalloc(size);

    if (ptr == NULL) {
        return 1; // L'allocation de mémoire a échoué
    } else {
        printf("Allocation de mémoire réussie.\n");
    }
	// N'oubliez pas de libérer la mémoire lorsque vous avez terminé avec elle.
    free(ptr);
	return 0; // Succès
}*/
