/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*       ft_printf.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 13:29:47 by sruzzi            #+#    #+#             */
/*   Updated: 2024/01/08 13:33:05 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef FT_PRINTF_H

# define FT_PRINTF_H

# include <stdarg.h>
# include <stddef.h>
# include "../libft.h"
# include <stdlib.h>
# include <unistd.h>
# include <stdint.h>

size_t		nb_len(long n);
char		*ft_strchr(char const *str, int c);
char		*ft_strchr(const char *str, int c);
char		*ft_itoa(int n);
int			ft_putstr(char *s);
int			ft_putchar(char c);
int			ft_putnbr(int number);
int			ft_printf(const char *format, ...);
int			print_format(char format, va_list argmt);

#endif
