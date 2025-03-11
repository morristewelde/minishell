/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_token.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtewelde <mtewelde@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 16:26:09 by sruzzi            #+#    #+#             */
/*   Updated: 2025/03/11 20:01:22 by mtewelde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/* Vérifie que la commande ne commence pas par un PIPE et qu'aucun token n'est UNSUPPORTED */
/* Vérifie que la commande ne commence pas par un token interdit */
static int	check_initial_tokens(t_token *tokens)
{
	if (!tokens)
		return (0);
	if (tokens->type == TOKEN_UNSUPPORTED)
	{
		fprintf(stderr, "Error: unsupported token detected\n");
		return (0);
	}
	if (tokens->type == TOKEN_PIPE)
		fprintf(stderr, "bash: syntax error near unexpected token '|'\n");
	else if (tokens->type == TOKEN_SEMICOLON)
		fprintf(stderr, "bash: syntax error near unexpected token ';'\n");
	else if (tokens->type == TOKEN_REDIRECT_IN || tokens->type == TOKEN_REDIRECT_OUT ||
			  tokens->type == TOKEN_REDIRECT_APPEND)
		fprintf(stderr, "bash: syntax error near unexpected token 'newline'\n");
	else
		return (1);
	return (0);
}

/* Vérifie si un token est un séparateur nécessitant un argument */
static int	is_separator_token(t_token *token)
{
	if (token->type == TOKEN_PIPE)
		return (1);
	if (token->type == TOKEN_REDIRECT_IN)
		return (1);
	if (token->type == TOKEN_REDIRECT_OUT)
		return (1);
	if (token->type == TOKEN_REDIRECT_APPEND)
		return (1);
	if (token->type == TOKEN_HEREDOC)
		return (1);
	if (token->type == TOKEN_SEMICOLON)
		return (1);
	return (0);
}


/* Vérifie les erreurs spécifiques au heredoc (<<) */
static int	check_heredoc_syntax(t_token *current)
{
	if (!current->next || current->next->type == TOKEN_PIPE || current->next->type == TOKEN_SEMICOLON)
	{
		fprintf(stderr, "bash: syntax error near unexpected token 'newline'\n");
		return (0);
	}
	if (current->next && current->next->next && current->next->next->type == TOKEN_WORD)
	{
		fprintf(stderr, "Error: unexpected token after heredoc delimiter\n");
		return (0);
	}
	return (1);
}
/*
** Gère spécifiquement les erreurs des `;` (TOKEN_SEMICOLON).
** - Vérifie qu'il n'y a pas `;;` consécutifs.
** - Vérifie que `;` n'est pas en fin de ligne.
*/
/* Vérifie les erreurs spécifiques au ; */
static int	handle_semicolon_token(t_token *current)
{
	if (!current->next)
	{
		fprintf(stderr, "bash: syntax error near unexpected token ';'\n");
		return (0);
	}
	if (current->next->type == TOKEN_SEMICOLON)
	{
		fprintf(stderr, "bash: syntax error near unexpected token ';;'\n");
		return (0);
	}
	return (1);
}
/* Gère les erreurs liées aux tokens séparateurs */
/* Gère les erreurs des séparateurs et les cas spéciaux heredoc/semicolon */
static int	handle_separator_errors(t_token *current, int is_separator)
{
	if (is_separator) 
	{
		fprintf(stderr, "bash: syntax error near unexpected token '%s'\n",
			current->value);
		return (0);
	}
	if (!current->next)
	{
		if (current->type == TOKEN_PIPE)
			fprintf(stderr, "bash: syntax error near unexpected token '|'\n");
		else if (current->type == TOKEN_SEMICOLON)
			fprintf(stderr, "bash: syntax error near unexpected token ';'\n");
		else
			fprintf(stderr, "bash: syntax error near unexpected token 'newline'\n");
		return (0);
	}
	if (current->type == TOKEN_HEREDOC && !check_heredoc_syntax(current))
		return (0);
	if (current->type == TOKEN_SEMICOLON && !handle_semicolon_token(current))
		return (0);
	return (1);
}


/* Fonction principale de validation des tokens */
int	validate_token_sequence(t_token *tokens)
{
	t_token	*current;
	int		is_separator;

	if (!check_initial_tokens(tokens))
		return (0);
	current = tokens;
	is_separator = 0;
	while (current)
	{
		if (is_separator_token(current))
		{
			if (!handle_separator_errors(current, is_separator))
				return (0);
			is_separator = 1;
		}
		else
			is_separator = 0;
		current = current->next;
	}
	return (1);
}

