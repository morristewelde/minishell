/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   organize_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtewelde <mtewelde@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 16:25:18 by sruzzi            #+#    #+#             */
/*   Updated: 2025/03/11 18:46:30 by mtewelde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
** 1) add_command_if_needed
** Crée une nouvelle commande si *current est NULL et l'insère
** à la fin de la liste chaînée *commands.
*/
static int	add_command_if_needed(t_command **commands, t_command **current, t_shell *shell)
{
	t_command	*tmp;

	if (!*current)
	{
		*current = init_new_command(shell);
		if (!*current)
		{
			print_error("organize_commands", "memory allocation failed");
			free_commands(*commands);
			return (0);
		}
		if (!*commands)
			*commands = *current;
		else
		{
			tmp = *commands;
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = *current;
		}
	}
	return (1);
}

/*
** 2) handle_word_token
** Gestion du token WORD : duplication et ajout à la commande.
*/
int	handle_word_token(t_command *cur, t_token *tok, t_shell *shell, t_command *cmds)
{
	char	*arg;
	
	// ✅ Ignore complètement les tokens vides rajout du 1O mars a voir si c bon 
	//if (!tok->value[0]) je lenleves car jai pas confiance
	//	return (1);
	arg = safe_strdup(tok->value, shell);
	if (!arg)
	{
		print_error("organize_commands", "memory allocation failed for argument");
		free_commands(cmds);
		return (0);
	}
	add_argument_to_command(cur, arg, shell);
	return (1);
}

/*
** 3) handle_pipe_token
** Gère le token PIPE : marque pipe_out, avance *tokens et crée
** éventuellement une nouvelle commande; renvoie 2 pour un 'continue' immédiat.
*/
int	handle_pipe_token(t_token **tokens, t_command **cur, t_command **cmds, t_shell *shell)
{
	t_command	*new_cmd;

	(*cur)->pipe_out = 1;
	*tokens = (*tokens)->next;
	if (*tokens)
	{
		new_cmd = init_new_command(shell);
		if (!new_cmd)
		{
			print_error("organize_commands", "memory allocation failed after pipe");
			free_commands(*cmds);
			return (0);
		}
		new_cmd->pipe_in = 1;
		(*cur)->next = new_cmd;
		*cur = new_cmd;
	}
	return (2);
}

/*
** 9) organize_commands
** Fonction principale (moins de 25 lignes) :
**  - Ajoute une nouvelle commande si nécessaire,
**  - Appelle handle_single_token pour traiter le token,
**  - Gère le retour 2 -> continue.
*/
t_command	*organize_commands(t_token *tokens, t_shell *shell)
{
	t_command	*commands;
	t_command	*current;
	int			ret;

	commands = NULL;
	current = NULL;
	while (tokens)
	{
		if (!add_command_if_needed(&commands, &current, shell))
			return (NULL);
		ret = handle_single_token(&tokens, &current, &commands, shell);
		if (ret == 0)
			return (NULL);
		if (ret == 2)
			continue ;
		tokens = tokens->next;
	}
	if (!commands)
	{
		print_error("organize_commands", "no valid commands detected");
		return (NULL);
	}
	return (commands);
}
