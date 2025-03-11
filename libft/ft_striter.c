/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <sruzzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 10:03:50 by sruzzi            #+#    #+#             */
/*   Updated: 2023/11/07 19:52:49 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striter(char *s, void (*f)(char *))
{
	if (s && f)
	{
		while (*s != 0)
		{
			f(s++);
		}
	}
}
/*
static void	print_char(char *c)
{
	printf("*%c-", *c);
}
int	main(void)
{
	char	str[] = "Hello";
	ft_striter(str, print_char);
	return (0);
}*/
