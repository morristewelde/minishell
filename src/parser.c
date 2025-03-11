#include "minishell.h"

//actuellement parse_token est inutile et pas appelee
t_command	*create_or_append_command(t_command **commands, t_command **current)
{
	if (!*current)
	{
		*current = malloc(sizeof(t_command));
		ft_bzero(*current, sizeof(t_command));
		if (!*commands) *commands = *current;
		else
		{
			t_command *temp = *commands;
			while (temp->next) temp = temp->next;
			temp->next = *current;
		}
	}
	return (*current);
}

t_command	*parse_tokens(t_token *tokens)
{
	t_command	*commands = NULL;
	t_command	*current = NULL;

	while (tokens)
	{
		create_or_append_command(&commands, &current);
		if (tokens->type == TOKEN_WORD)
		{
			int argc = array_len(current->argv);
			current->argv = realloc(current->argv, sizeof(char *) * (argc + 2));
			current->argv[argc] = ft_strdup(tokens->value); current->argv[argc + 1] = NULL;
		}
		else if (tokens->type == TOKEN_REDIRECT_OUT && tokens->next)
			{ current->output_file = ft_strdup(tokens->next->value); tokens = tokens->next; }
		else if (tokens->type == TOKEN_PIPE) current = NULL;
		tokens = tokens->next;
	}
	return (commands);
}
