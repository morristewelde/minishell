/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*     ft_printf_utilsC.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 17:36:41 by sruzzi            #+#    #+#             */
/*   Updated: 2024/01/08 17:45:11 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

int	ft_putstr(char *str)
{
	if (str == NULL)
		return (write (1, "(null)", 6));
	else
		return (write(1, str, ft_strlen(str)));
}

int	ft_putchar(char c)
{
	ssize_t	ret;

	ret = write(1, &c, 1);
	if (ret == -1)
	{
		return (-1);
	}
	return (1);
}

int	ft_putnbr(int number)
{
	char	*num;
	int		len;

	len = 0;
	num = ft_itoa (number);
	len += ft_putstr (num);
	free(num);
	return (len);
}
/*
char	*ft_strchr(char const *str, int c)
{
	while (*str)
	{
		if (*str == (char)c)
		{
			return ((char *)str);
		}
		str++;
	}
	if (*str == (char)c)
		return ((char *)str);
	else
		return (NULL);
}
*/
