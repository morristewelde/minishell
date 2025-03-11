/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handles_quotes_exp.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 15:44:54 by sruzzi            #+#    #+#             */
/*   Updated: 2025/03/07 15:45:55 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"
/*
** Traite un segment entre quotes.
** Si c'est une double quote, on effectue l'expansion des variables.
*/
static char	*process_quoted(const char *input, int *i, t_shell *shell)
{
	char			*sub;
	int				start;
	t_quote_type	quote_state;

	quote_state = set_in_quotes(input[*i], NO_QUOTE);
	(*i)++; // on saute la quote ouvrante
	start = *i;
	while (input[*i] && set_in_quotes(input[*i], quote_state) == quote_state)
		(*i)++;
	if (!input[*i])
		return (NULL);
	sub = ft_substr(input, start, *i - start);
	if (!sub)
		return (NULL);
	if (quote_state == DOUBLE_QUOTE)
		sub = replace_env_variables(sub, shell);
	(*i)++; // on saute la quote fermante
	return (sub);
}

/*
** Traite l'expansion d'une variable commençant par '$'.
*/
static char	*process_dollar(const char *input, int *i, t_shell *shell)
{
	char	*sub;
	int		start;

	start = *i;
	while (input[++(*i)] && (ft_isalnum(input[*i])
			|| input[*i] == '_' || input[*i] == '?'))
		;
	sub = ft_substr(input, start, *i - start);
	sub = replace_env_variables(sub, shell);
	return (sub);
}

/*
** Récupère le prochain token (segment entre quotes, expansion ou caractère simple).
*/
static char	*get_next_token(const char *input, int *i, t_shell *shell)
{
	char	*token;

	if (input[*i] == '\'' || input[*i] == '"')
		token = process_quoted(input, i, shell);
	else if (input[*i] == '$' && (ft_isalnum(input[*i + 1])
			|| input[*i + 1] == '_' || input[*i + 1] == '?'))
		token = process_dollar(input, i, shell);
	else
	{
		char	temp[2] = { input[*i], '\0' };
		token = safe_strdup(temp, shell);
		(*i)++;
	}
	return (token);
}

/*
** Traite l'ensemble des quotes et expansions dans l'input.
** Chaque token est extrait via get_next_token() et concaténé au résultat.
*/
char	*process_quotes_and_expansion(const char *input, t_shell *shell)
{
	char	*result;
	char	*token;
	int		i;

	result = safe_strdup("", shell);
	if (!result)
		return (NULL);
	i = 0;
	while (input[i])
	{
		token = get_next_token(input, &i, shell);
		if (!token)
		{
			free(result);
			return (NULL);
		}
		result = ft_strjoin_multiple(shell, 2, result, token);
		free(token);
		if (!result)
			return (NULL);
	}
	return (result);
}
