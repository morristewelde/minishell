#include "../include/minishell.h"

char	*read_user_input(t_shell *shell)
{
	char	*prompt;
	char	*input;

	if (isatty(STDIN_FILENO)) // Mode interactif
	{
		prompt = get_prompt(shell);
		if (!prompt)
		{
			perror("Failed to generate prompt");
			return (NULL);
		}
		input = readline(prompt);
		free(prompt);
	}
	else // Mode pipe
	{
		input = get_next_line(STDIN_FILENO);
		if (input) // Supprimer le \n de fin s'il y en a un
		{
			size_t len = ft_strlen(input);
			if (len > 0 && input[len - 1] == '\n')
				input[len - 1] = '\0';
		}
	}

	if (!input) // EOF ou Ctrl+D
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

void process_input(t_shell *shell)
{
	char *input;
	t_token *tokens;
    t_command *commands;

    // 1) Read user input
    input = read_user_input(shell);
    if (!input || !shell->running)
        return;

    // 2) Handle the special case of `:`
    if (ft_strcmp(input, ":") == 0 || ft_strcmp(input, "!") == 0) {
        free(input);
        return; // No-op, just return without doing anything
    }

    // 3) Tokenize the input
    tokens = tokenize_input(input, shell);
    if (!tokens) {
        free(input);
        return;
    }

    // 4) Validate the token sequence
    if (!validate_token_sequence(tokens)) {
        token_free(&tokens);
        free(input);
        return;
    }

    // 5) Organize the commands
    commands = organize_commands(tokens, shell);
    token_free(&tokens);
    if (!commands) {
        free(input);
        return;
    }

    // 6) Validate the commands
    if (validate_commands(commands) < 0) {
        free_commands(commands);
        free(input);
        return;
    }

    // 7) Execute the commands
    execute_commands(shell, commands);

    // 8) Clean up
    free_commands(commands);
    free(input);
}