/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 15:23:10 by sruzzi            #+#    #+#             */
/*   Updated: 2025/03/07 15:23:37 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

/* ************************************************************************** */
/*                      LECTURE DE L'ENTRÉE UTILISATEUR                       */
/* ************************************************************************** */

/* Lis l'entrée utilisateur via readline et retourne la commande */
static char	*get_input(t_shell *shell)
{
	char	*input;
	char	*prompt;
	size_t	len;

	if (isatty(STDIN_FILENO))
	{
		prompt = get_prompt(shell);
		if (!prompt) { perror("Failed to generate prompt"); return (NULL); }
		input = readline(prompt);
		free(prompt);
	}
	else
	{
		input = get_next_line(STDIN_FILENO);
		if (input)
		{
			len = ft_strlen(input);
			if (len > 0 && input[len - 1] == '\n')
				input[len - 1] = '\0';
		}
	}
	return (input);
}

char	*read_user_input(t_shell *shell)
{
	char	*input;

	input = get_input(shell);
	if (!input)
	{
		if (isatty(STDIN_FILENO))
			printf("exit\n");
		shell->running = 0;
		return (NULL);
	}
	if (*input)
		add_history(input);
	return (input);
}

/* ************************************************************************** */
/*                   TRAITEMENT ET AFFICHAGE DES TOKENS                       */
/* ************************************************************************** */

/* Processus de gestion d'une commande */

static t_token	*prepare_tokens(char *input, t_shell *shell)
{
	t_token	*tokens = tokenize_input(input, shell);

	if (!tokens || !validate_token_sequence(tokens))
	{
		if (tokens)
			token_free(&tokens);
		return (NULL);
	}
	return (tokens);
}

static t_command	*prepare_commands(t_token *tokens, t_shell *shell, char *input)
{
	t_command	*commands = organize_commands(tokens, shell);

	token_free(&tokens);
	if (!commands || validate_commands(commands) < 0)
	{
		if (commands)
			free_commands(commands);
		free(input);
		return (NULL);
	}
	return (commands);
}

void	process_input(t_shell *shell)
{
	char		*input;
	t_token		*tokens;
	t_command	*commands;

	input = read_user_input(shell);
	if (!input || !shell->running)
		return ;
	if (!(tokens = prepare_tokens(input, shell)))
	{
		free(input);
		return ;
	}
	if (!(commands = prepare_commands(tokens, shell, input)))
		return ;
	execute_commands(shell, commands);
	free_commands(commands);
	free(input);
}

