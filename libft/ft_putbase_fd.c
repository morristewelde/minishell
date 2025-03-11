/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putbase_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <sruzzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 11:05:50 by sruzzi            #+#    #+#             */
/*   Updated: 2023/11/07 20:21:28 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putbase_fd(int nbr, const char *base, int fd)
{
	long		nb;
	int			base_len;
	char		c;

	base_len = ft_strlen(base);
	if (nbr < 0)
	{
		write (fd, "-", 1);
		nb = -((long)nbr);
	}
	else
	{
		nb = (long)nbr;
	}
	if (nb > (base_len - 1))
		ft_putbase_fd(nb / base_len, base, fd);
	c = *(base + (nb % base_len));
	write (fd, &c, 1);
}
