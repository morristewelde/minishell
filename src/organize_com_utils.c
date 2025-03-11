/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   organize_com_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 16:25:00 by sruzzi            #+#    #+#             */
/*   Updated: 2025/03/07 16:25:07 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

/*
** 4) handle_redirect_in_token
** Affecte le fichier d'entrée à la commande, avec vérification mémoire.
*/
static int	handle_redirect_in_token(t_token **tokens, t_command *cur, t_shell *shell, t_command *cmds)
{
	cur->input_file = safe_strdup((*tokens)->next->value, shell);
	if (!cur->input_file)
	{
		print_error("organize_commands", "memory allocation failed for input_file");
		free_commands(cmds);
		return (0);
	}
	*tokens = (*tokens)->next;
	return (1);
}

/*
** 5) handle_redirect_out_token
** Affecte le fichier de sortie à la commande, avec vérification mémoire.
*/
static int	handle_redirect_out_token(t_token **tokens, t_command *cur, t_shell *shell, t_command *cmds)
{
	cur->output_file = safe_strdup((*tokens)->next->value, shell);
	if (!cur->output_file)
	{
		print_error("organize_commands", "memory allocation failed for output_file");
		free_commands(cmds);
		return (0);
	}
	cur->append = ((*tokens)->type == TOKEN_REDIRECT_APPEND);
	*tokens = (*tokens)->next;
	return (1);
}

/*
** 6) handle_heredoc_token
** Affecte le délimiteur heredoc à la commande, avec vérification mémoire.
*/
static int	handle_heredoc_token(t_token **tokens, t_command *cur, t_shell *shell, t_command *cmds)
{
	cur->heredoc_delimiter = safe_strdup((*tokens)->next->value, shell);
	if (!cur->heredoc_delimiter)
	{
		print_error("organize_commands", "memory allocation failed for heredoc_delimiter");
		free_commands(cmds);
		return (0);
	}
	cur->heredoc = 1;
	*tokens = (*tokens)->next;
	return (1);
}
/*
** 7) handle_semicolon_token
** Gère le token `;` (point-virgule).
** - Sépare les commandes.
** - Retourne 2 pour forcer un `continue;` dans `organize_commands`.
*/
static int	handle_semicolon_token(t_token **tokens, t_command **cur)
{
	*cur = NULL; // 🔥 Force `organize_commands` à créer une nouvelle commande
	*tokens = (*tokens)->next;
	return (2);
}

/*
** 8) handle_single_token
** Route vers la fonction adaptée selon le type de token.
** Retourne 0 en cas d'erreur, 2 pour un PIPE, 1 sinon.
*/
int	handle_single_token(t_token **tokens, t_command **cur, t_command **cmds, t_shell *shell)
{
	if ((*tokens)->type == TOKEN_WORD)
		return (handle_word_token(*cur, *tokens, shell, *cmds));
	else if ((*tokens)->type == TOKEN_PIPE)
		return (handle_pipe_token(tokens, cur, cmds, shell));
	else if ((*tokens)->type == TOKEN_REDIRECT_IN)
		return (handle_redirect_in_token(tokens, *cur, shell, *cmds));
	else if ((*tokens)->type == TOKEN_REDIRECT_OUT || (*tokens)->type == TOKEN_REDIRECT_APPEND)
		return (handle_redirect_out_token(tokens, *cur, shell, *cmds));
	else if ((*tokens)->type == TOKEN_HEREDOC)
		return (handle_heredoc_token(tokens, *cur, shell, *cmds));
	else if ((*tokens)->type == TOKEN_SEMICOLON)
		return (handle_semicolon_token(tokens, cur));
	return (1);
}
