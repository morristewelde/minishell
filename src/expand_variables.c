/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 15:44:05 by sruzzi            #+#    #+#             */
/*   Updated: 2025/03/07 15:44:20 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

char	*replace_env_variables(const char *input, t_shell *shell)
{
	char	*result;
	int		i;

	result = safe_strdup("", shell); // 🔥 Utilisation sécurisée de safe_strdup
	if (!result)
		return (NULL); // On propage l'erreur proprement
	i = 0;
	while (input[i])
	{
		if (input[i] == '$' && (ft_isalnum(input[i + 1]) || input[i + 1] == '_' || input[i + 1] == '?'))
		{
			result = replace_variable(input, &i, result, shell);
			if (!result)
				return (NULL); // On arrête tout si erreur mémoire
		}
		else
		{
			result = append_char(result, input[i]);
			if (!result)
				return (NULL); // Même chose ici
			i++;
		}
	}
	return (result);
}

char	*append_char(char *result, char c)
{
	char	temp[2];
	char	*new_result;

	temp[0] = c;
	temp[1] = '\0';
	new_result = ft_strjoin(result, temp);
	free(result);
	if (!new_result)
		printf("[DEBUG] append_char: Failed to append character\n");
	return (new_result);
}

/*
** Extraction de la valeur de la variable.
** Si c'est "$?", on renvoie l'itoa de shell->exit_stat.
** Sinon, on extrait le nom et on récupère sa valeur dans l'environnement.
*/
static char	*get_var_value(const char *input, int *i, t_shell *shell)
{
	char	*var_value;
	char	*var_name;
	int		var_start;

	(*i)++;
	if (input[*i] == '?')
	{
		var_value = ft_itoa(shell->exit_stat);
		(*i)++;
		return (var_value);
	}
	var_start = *i;
	while (ft_isalnum(input[*i]) || input[*i] == '_')
		(*i)++;
	var_name = ft_substr(input, var_start, *i - var_start);
	if (!var_name)
		return (NULL);
	var_value = get_env_value(shell->env, var_name);
	free(var_name);
	return (var_value);
}

/*
** Remplace la variable dans l'input et l'ajoute au résultat.
*/
char	*replace_variable(const char *input, int *i, char *result, t_shell *shell)
{
	char	*var_value;
	char	*temp;

	var_value = get_var_value(input, i, shell);
	if (!var_value)
		var_value = safe_strdup("", shell);
	temp = ft_strjoin(result, var_value);
	free(result);
	free(var_value);
	return (temp);
}
