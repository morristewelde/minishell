#include "../include/minishell.h"

void	debug_commands(t_command *commands)
{
	t_command	*cmd;
	int			i;

	printf("\n=== DEBUG COMMANDS ===\n");
	cmd = commands;
	while (cmd)
	{
		printf("Command:\n");
		printf("  Arguments:\n");
		for (i = 0; cmd->argv && cmd->argv[i]; i++)
			printf("    argv[%d]: '%s'\n", i, cmd->argv[i]);

		if (cmd->input_file)
			printf("  Input file: '%s'\n", cmd->input_file);
		else
			printf("  Input file: None\n");

		if (cmd->output_file)
			printf("  Output file: '%s'\n", cmd->output_file);
		else
			printf("  Output file: None\n");

		if (cmd->heredoc_delimiter)
			printf("  Heredoc Delimiter: '%s'\n", cmd->heredoc_delimiter);
		else
			printf("  Heredoc Delimiter: None\n");

		printf("  Append: %d\n", cmd->append);
		printf("  Pipe In: %d\n", cmd->pipe_in);
		printf("  Pipe Out: %d\n", cmd->pipe_out);
		printf("-----------------------\n");

		cmd = cmd->next;
	}
	printf("=== END OF COMMANDS ===\n\n");
}

/* Fonction pour afficher un debug des tokens générés */
void	debug_tokens(t_token *tokens)
{
	const char	*token_type_str[] = {
		"TOKEN_WORD",
		"TOKEN_PIPE",
		"TOKEN_REDIRECT_IN",
		"TOKEN_REDIRECT_OUT",
		"TOKEN_REDIRECT_APPEND",
		"TOKEN_HEREDOC"
	};

	t_token	*current = tokens;

	printf("[DEBUG] Tokens generated:\n");
	while (current)
	{
		printf("  [TOKEN] Type: %s, Value: '%s'\n",
			(current->type >= 0 && current->type <= TOKEN_HEREDOC) ? token_type_str[current->type] : "UNKNOWN",
			current->value);
		current = current->next;
	}
	printf("[DEBUG] End of Tokens.\n\n");
}

