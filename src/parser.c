#include "../include/minishell.h"

//actuellement parse_token est inutile et pas appelee
t_command	*parse_tokens(t_token *tokens)
{
	t_command	*commands = NULL;
	t_command	*current = NULL;

	while (tokens)
	{
		if (!current)
		{
			current = malloc(sizeof(t_command));
			if (!current)
				return (NULL);
			ft_bzero(current, sizeof(t_command));
			if (!commands)
				commands = current;
			else
			{
				t_command *temp = commands;
				while (temp->next)
					temp = temp->next;
				temp->next = current;
			}
		}
		if (tokens->type == TOKEN_WORD)
		{
			int argc = array_len(current->argv);
			current->argv = realloc(current->argv, sizeof(char *) * (argc + 2));
			current->argv[argc] = ft_strdup(tokens->value);
			current->argv[argc + 1] = NULL;
		}
		else if (tokens->type == TOKEN_REDIRECT_OUT && tokens->next)
		{
			current->output_file = ft_strdup(tokens->next->value);
			tokens = tokens->next;
		}
		else if (tokens->type == TOKEN_PIPE)
			current = NULL;
		tokens = tokens->next;
	}
	return (commands);
}
/*
char	*handle_heredoc(const char *delimiter)
{
	char	*line;
	char	*content = ft_strdup("");

	if (!content)
		return (NULL);
	while (1)
	{
		line = readline("minishell> ");
		if (!line || ft_strcmp(line, delimiter) == 0) // Délimiteur rencontré
			break;
		char *temp = content;
		content = ft_strjoin(content, line);
		content = ft_strjoin(content, "\n");
		free(temp);
		free(line);
	}
	free(line);
	return (content);
}
*/

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

