/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 16:36:35 by sruzzi            #+#    #+#             */
/*   Updated: 2025/03/07 16:36:42 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

char	*safe_substr(const char *s, unsigned int start, size_t len, t_shell *shell)
{
	char	*substr;

	if (!s)
		return (NULL);
	substr = ft_substr(s, start, len);
	if (!substr)
	{
		print_error("substr", "memory allocation failed");
		shell->exit_stat = 1;
		return (NULL);
	}
	return (substr);
}

void	*safe_malloc(size_t size, t_shell *shell)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
	{
		print_error("malloc", "memory allocation failed");
		shell->exit_stat = 1;
		return (NULL);
	}
	return (ptr);
}

char	*safe_strdup(const char *s, t_shell *shell)
{
	char	*copy;

	copy = ft_strdup(s);
	if (!copy)
	{
		print_error("strdup", "memory allocation failed");
		shell->exit_stat = 1;
		return (NULL);
	}
	return (copy);
}

void	*safe_realloc(void *ptr, size_t old_size, size_t new_size, t_shell *shell)
{
	void	*new_ptr;
	size_t	i;

	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	if (!ptr)
		return (safe_malloc(new_size, shell));
	new_ptr = safe_malloc(new_size, shell);
	if (!new_ptr)
		return (NULL);
	i = 0;
	while (i < old_size && i < new_size)
	{
		((char *)new_ptr)[i] = ((char *)ptr)[i];
		i++;
	}
	free(ptr);
	return (new_ptr);
}
