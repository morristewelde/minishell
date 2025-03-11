/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 15:54:36 by sruzzi            #+#    #+#             */
/*   Updated: 2025/03/07 15:54:42 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

char	*ft_strjoin_three(const char *s1, const char *s2, const char *s3, t_shell *shell)
{
	size_t	len;
	char	*result;

	if (!s1 || !s2 || !s3)
	{
		print_error("ft_strjoin_three", "NULL argument provided");
		return (NULL);
	}

	len = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3) + 1;
	result = safe_malloc(len, shell);
	if (!result)
		return (NULL);

	ft_memset(result, 0, len);
	ft_strlcpy(result, s1, len);
	ft_strlcat(result, s2, len);
	ft_strlcat(result, s3, len);
	return (result);
}

char	*ft_strjoin_multiple(t_shell *shell, int count, ...)
{
	va_list	args;
	char	*result;
	char	*temp;
	size_t	total_len;
	int		i;

	total_len = 1;
	va_start(args, count);
	i = 0;
	while (i < count)
	{
		const char	*current;

		current = va_arg(args, const char *);
		if (current)
			total_len += ft_strlen(current);
		i++;
	}
	va_end(args);
	result = safe_malloc(total_len, shell);
	if (!result)
		return (NULL);
	*result = '\0';
	va_start(args, count);
	i = 0;
	while (i < count)
	{
		temp = va_arg(args, char *);
		if (!temp)
			temp = "";
		ft_strlcat(result, temp, total_len);
		i++;
	}
	va_end(args);
	return (result);
}



// Retourne la longueur d'un tableau 2D (null-terminated)
int	array_len(char **array)
{
	int	len;

	len = 0;
	while (array && array[len])
		len++;
	return (len);
}

// Libère un tableau 2D (null-terminated)
void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}
