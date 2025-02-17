/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <sruzzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 11:19:56 by sruzzi            #+#    #+#             */
/*   Updated: 2023/11/07 19:20:01 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] != '\0')
	{
		if (str[i] >= '0' && str[i] <= '9')
			result = result * 10 + str[i] - '0';
		else
			break ;
		i++;
	}
	return (result * sign);
}
/*
int	main(void)
{
	const char *input = "   -42"; // Remplacez ceci par la chaî
	ne que vous souhaitez convertir
	int result1 = ft_atoi(input);
	printf("La conversion de la chaîne \"%s\" en entier donne :
	 %d\n", input, result1);
	const char *inputput = " -424242042";
	int result2 = ft_atoi(inputput);
	printf("La conversion de la chaîne \"%s\" en entier donne :
	 %d\n", inputput, result2);
	return (0);
}*/
