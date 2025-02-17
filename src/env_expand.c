#include "../include/minishell.h"

/* ************************************************************************** */
/*                           FONCTION PRINCIPALE                              */
/* ************************************************************************** */

char	*replace_env_variables(const char *input, t_shell *shell)
{
	char	*result;
	int		i;

	if (!input) // Petit garde-fou
		return (NULL);

	result = safe_strdup("", shell);
	if (!result)
		return (NULL);

	i = 0;
	while (input[i])
	{
		if (input[i] == '$'
			&& (ft_isalnum(input[i + 1]) || input[i + 1] == '_' || input[i + 1] == '?'))
		{
			result = replace_variable(input, &i, result, shell);
			if (!result)
				return (NULL); // On sort direct si erreur (safe_strdup aura déjà mis shell->exit_stat = 1)
		}
		else
		{
			char *tmp = append_char(result, input[i]);
			if (!tmp)
			{
				free(result); // on libère l’ancien result
				return (NULL);
			}
			result = tmp;
			i++;
		}
	}

	//printf("[DEBUG] Expansion de `%s` → `%s`\n", input, result); // 🔍 Debug
	return (result);
}
char	*replace_variable(const char *input, int *i, char *result, t_shell *shell)
{
	int		var_start;
	char	*var_name = NULL;
	char	*var_value = NULL;
	char	*temp;

	(*i)++;
	// Cas spécial pour `$?`
	if (input[*i] == '?')
	{
		var_value = ft_itoa(shell->exit_stat);
		(*i)++; // Avancer après '?'
	}
	else
	{
		var_start = *i;
		while (ft_isalnum(input[*i]) || input[*i] == '_')
			(*i)++;
		var_name = ft_substr(input, var_start, *i - var_start);
		if (!var_name)
		{
			free(result);
			return (NULL);
		}
		var_value = get_env_value(shell->env, var_name);
		free(var_name);
	}

	// Correction mémoire : Dupliquer la valeur pour éviter un `double free`
	if (!var_value)
	{
		var_value = safe_strdup("", shell); // Éviter NULL
	}
	else
	{
		var_value = ft_strdup(var_value); // Créer une copie indépendante
	}

	// Vérification si `var_value` a bien été dupliqué
	if (!var_value)
	{
		free(result);
		return (NULL);
	}

	// DEBUG avancé avant le `free`
	//printf("[DEBUG] Expansion `$%s` → `%s` (ptr: %p)\n", var_name ? var_name : "(null)", var_value, (void *)var_value);

	temp = ft_strjoin(result, var_value);

	// Vérification avant de `free`
	if (var_value)
	{
		//printf("[DEBUG] Avant free: `%s` (ptr: %p)\n", var_value, (void *)var_value);
		free(var_value);
	}

	free(result);

	if (!temp)
		return (NULL);

	return (temp);
}

char	*append_char(char *result, char c)
{
	char	temp[2];
	char	*new_result;

	if (!result)
		return (NULL);
	temp[0] = c;
	temp[1] = '\0';

	new_result = ft_strjoin(result, temp);
	free(result);

	if (!new_result)
	{
		fprintf(stderr, "[DEBUG] append_char: Failed to append character\n");
		return (NULL);
	}
	return (new_result);
}

char	*process_quotes_and_expansion(const char *input, t_shell *shell)
{
	t_quote_type	quote_state;
	char			*result;
	int				i;

	if (!input)
		return (NULL);

	result = safe_strdup("", shell);
	if (!result)
		return (NULL);

	quote_state = NO_QUOTE;
	i = 0;
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '"')
		{
			quote_state = set_in_quotes(input[i], quote_state);
			int start = ++i;
			while (input[i] && set_in_quotes(input[i], quote_state) == quote_state)
				i++;

			if (!input[i])
			{
				free(result);
				return (NULL); // guillemets non fermés
			}

			// Extraire la sous-chaîne brute
			char *sub = ft_substr(input, start, i - start);
			if (!sub)
			{
				free(result);
				return (NULL);
			}

			// Selon SINGLE ou DOUBLE
			if (quote_state == SINGLE_QUOTE)
			{
				// Juste concaténer tel quel
				char *tmp = ft_strjoin_multiple(shell, 2, result, sub);
				free(result);
				if (!tmp)
				{
					free(sub);
					return (NULL);
				}
				free(sub);
				result = tmp;
			}
			else if (quote_state == DOUBLE_QUOTE)
			{
				// Remplacer variables
				char *expanded = replace_env_variables(sub, shell);
				free(sub);
				if (!expanded)
				{
					free(result);
					return (NULL);
				}
				char *tmp = ft_strjoin_multiple(shell, 2, result, expanded);
				free(expanded);
				free(result);
				if (!tmp)
					return (NULL);
				result = tmp;
			}

			quote_state = set_in_quotes(input[i++], quote_state);
		}
		else if (input[i] == '$'
			&& (ft_isalnum(input[i + 1]) || input[i + 1] == '_' || input[i + 1] == '?'))
		{
			int start = i;
			while (input[++i] && (ft_isalnum(input[i]) || input[i] == '_' || input[i] == '?'))
				;
			char *sub = ft_substr(input, start, i - start);
			if (!sub)
			{
				free(result);
				return (NULL);
			}
			char *expanded = replace_env_variables(sub, shell);
			free(sub);
			if (!expanded)
			{
				free(result);
				return (NULL);
			}
			char *tmp = ft_strjoin_multiple(shell, 2, result, expanded);
			free(expanded);
			free(result);
			if (!tmp)
				return (NULL);
			result = tmp;
		}
		else
		{
			char temp[2] = { input[i++], '\0' };
			char *tmp = ft_strjoin_multiple(shell, 2, result, temp);
			free(result);
			if (!tmp)
				return (NULL);
			result = tmp;
		}
	}
	//printf("[DEBUG] Résultat final après quotes & expansion: `%s`\n", result);
	return (result);
}

