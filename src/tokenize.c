/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtewelde <mtewelde@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 15:52:21 by sruzzi            #+#    #+#             */
/*   Updated: 2025/03/11 18:39:34 by mtewelde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Gère le cas des redirections doubles (<< ou >>).
** Retourne l'indice mis à jour, ou -1 en cas d'erreur.
*/
static int	process_double_redirect(const char *input, int i,
		t_token **tokens, t_shell *shell)
{
	int	type;
	int	start;

	(void)shell;
	if (input[i] == '<')
		type = TOKEN_HEREDOC;
	else
		type = TOKEN_REDIRECT_APPEND;
	token_add(tokens, token_new(ft_substr(input, i, 2), type, NO_QUOTE));
	i += 2;
	if (type == TOKEN_HEREDOC)
	{
		i = get_next_index(input, i);  /* On saute les espaces avant le délimiteur */
		start = i;
		if (input[i] == '\'' || input[i] == '"')
			i = get_end_of_quotes(input, i);
		else
			i = get_end_of_argument(input, i);
		if (i == -1)
			return (-1);
		token_add(tokens, token_new(ft_substr(input, start, i - start),
				TOKEN_WORD, NO_QUOTE));
	}
	return (i);
}

/*
** Gère le cas des redirections simples (< ou >).
** Retourne l'indice mis à jour.
*/
static int	process_single_redirect(const char *input, int i,
		t_token **tokens, t_shell *shell)
{
	(void)shell;
	if (input[i] == '<')
		token_add(tokens, token_new(ft_substr(input, i, 1),
				TOKEN_REDIRECT_IN, NO_QUOTE));
	else
		token_add(tokens, token_new(ft_substr(input, i, 1),
				TOKEN_REDIRECT_OUT, NO_QUOTE));
	i++;
	return (i);
}

static int	handle_quotes(const char *input, int i, t_quote_type *quote_type)
{
	if (input[i] == '\'')
	{
		*quote_type = SINGLE_QUOTE;
		return (get_end_of_quotes(input, i));
	}
	else if (input[i] == '"')
	{
		*quote_type = DOUBLE_QUOTE;
		return (get_end_of_quotes(input, i));
	}
	else
	{
		*quote_type = NO_QUOTE;
		return (get_end_of_argument(input, i));
	}
}
static int	merge_tokens(t_token *last_token, char *processed_value)
{
	char	*new_value;

	new_value = ft_strjoin(last_token->value, processed_value);
	if (!new_value)
	{
		free(processed_value);
		return (-1);
	}
	free(last_token->value);
	last_token->value = new_value;
	free(processed_value);
	return (0);
}
static int	process_semicolon(int i, t_token **tokens)
{
	token_add(tokens, token_new(";", TOKEN_SEMICOLON, NO_QUOTE));
	return (i + 1);
}


static int	process_word(const char *input, int i, t_token **tokens, t_shell *shell)
{
	int			start;
	char		*processed_value;
	t_token		*last_token;
	t_quote_type	quote_type;

	start = i;
	i = handle_quotes(input, i, &quote_type); // Appel de handle_quotes
	if (i == -1) // Vérification de l'erreur
		return (-1);
	processed_value = process_quotes_and_expansion(ft_substr(input, start, i - start), shell);
	if (!processed_value)
		return (-1);
	last_token = get_last_token(*tokens);
	if (last_token && last_token->type == TOKEN_WORD && last_token->quote != NO_QUOTE)
	{
		if (merge_tokens(last_token, processed_value) == -1)
			return (-1);
	}
	else
		token_add(tokens, token_new(processed_value, TOKEN_WORD, quote_type));
	return (i);
}


/*
** Dispatch pour gérer le token courant.
** Retourne l'indice mis à jour, ou -1 en cas d'erreur.
*/
static int	handle_token(const char *input, int i,
		t_token **tokens, t_shell *shell)
{
	if ((input[i] == '&' && input[i + 1] == '&')
		|| (input[i] == '|' && input[i + 1] == '|'))
	{
		token_add(tokens, token_new(ft_substr(input, i, 2),
				TOKEN_UNSUPPORTED, NO_QUOTE));
		return (i + 2);
	}
	if (input[i] == '|')
	{
		token_add(tokens, token_new(ft_substr(input, i, 1),
				TOKEN_PIPE, NO_QUOTE));
		return (i + 1);
	}
	if (input[i] == ';')
		return (process_semicolon(i, tokens));
	if (input[i] == '<' || input[i] == '>')
	{
		if (input[i + 1] == input[i])
			return (process_double_redirect(input, i, tokens, shell));
		else
			return (process_single_redirect(input, i, tokens, shell));
	}
	return (process_word(input, i, tokens, shell));
}

/*
** Fonction principale de tokenisation.
** Retourne la liste chaînée de tokens ou NULL en cas d'erreur.
*/
t_token	*tokenize_input(const char *input, t_shell *shell)
{
	t_token	*tokens;
	int		i;

	tokens = NULL;
	i = 0;
	if (!check_quotes(input))
		return (NULL);
	while (input[i])
	{
		i = get_next_index(input, i);
		if (!input[i])
			break ;
		i = handle_token(input, i, &tokens, shell);
		if (i == -1)
		{
			token_free(&tokens);
			return (NULL);
		}
	}
	return (tokens);
}
