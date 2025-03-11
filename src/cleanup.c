/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 16:36:48 by sruzzi            #+#    #+#             */
/*   Updated: 2025/03/07 16:36:55 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void cleanup_shell(t_shell *shell, t_command *commands, t_token *tokens, char *user_input)
{
	if (g_child_pid > 0)
	{
		kill(g_child_pid, SIGKILL);
		g_child_pid = -1;
	}
	if (shell->env)
	{
		free_env(shell->env);
		shell->env = NULL;
	}
	if (commands)
	{
		free_commands(commands);
		commands = NULL;
	}
	if (tokens)
	{
		token_free(&tokens);
		tokens = NULL;
	}
	if (user_input)
	{
		free(user_input);
		user_input = NULL;
	}
}

void	free_commands(t_command *commands)
{
	t_command	*temp;

	while (commands)
	{
		// Sauvegarder le pointeur vers le nœud suivant
		temp = commands->next;

		// Libérer les arguments de la commande (tableau de chaînes)
		if (commands->argv)
			free_array(commands->argv);

		// Libérer les fichiers d'entrée et de sortie
		free(commands->input_file);
		free(commands->output_file);
		free(commands->heredoc_delimiter); // Libérer le délimiteur
		// Libérer la structure elle-même
		free(commands);

		// Passer au nœud suivant
		commands = temp;
	}
}
