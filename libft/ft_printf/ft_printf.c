/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*       ft_printf.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 11:29:08 by sruzzi            #+#    #+#             */
/*   Updated: 2024/01/08 11:30:42 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

int	print_format(char format, va_list argmt)
{
	if (format == 'c')
		return (ft_putchar(va_arg(argmt, int)));
	else if (format == 's')
		return (ft_putstr(va_arg(argmt, char *)));
	else if (format == 'd' || format == 'i')
		return (ft_putnbr(va_arg(argmt, int)));
	return (0);
}

int	ft_printf(const char *format, ...)
{
	va_list	argmt;
	int		count;

	va_start (argmt, format);
	count = 0;
	while (*format != '\0')
	{
		if (*format == '%')
		{
			format++;
			if (ft_strchr("csdi", *format))
				count += print_format(*format, argmt);
			else if (*format == '%')
				count += write (1, format, 1);
		}
		else
			count += write (1, format, 1);
		++format;
	}
	va_end (argmt);
	return (count);
}
