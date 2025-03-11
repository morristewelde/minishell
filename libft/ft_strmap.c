/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <sruzzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 11:03:45 by sruzzi            #+#    #+#             */
/*   Updated: 2023/11/07 20:19:41 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmap(const char *s, char (*f) (char))
{
	int				i;
	size_t			len;
	char			str;

	if (!s)
		return (NULL);
	i = 0;
	len = ft_strlen(s);
	str = (char *) malloc (sizeof(*s) * len + 1);
	if (!str)
		return (NULL);
	while (s[i])
	{
		str[i] = f(s[i]);
		i++;
	}
	str[i] = '\0';
	return (str);
}

/*
char my_function(char c)
{   
       if (c >= 'a' && c <= 'z') {
        return c - 'a' + 'A';
    }
    return c;  // Renvoie le caractère tel quel s'il n'est pas une minuscule
}

int	main(void)
{
    const char *input_string = "Hello, World!";
    
 
    char *result = ft_strmap(input_string, my_function);

    if (result) {
        printf("Chaîne d'origine : %s\n", input_string);
        printf("Chaîne transformée : %s\n", result);
        free(result); // Assurez-vous de libérer la mémoire allouée dynamiquement
    } else {
        printf("Erreur d'allocation de mémoire.\n");
    }

    return 0;
}*/
