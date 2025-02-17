#include "../include/minishell.h"

int validate_token_sequence(t_token *tokens)
{
    t_token *segment_start;
    t_token *current;
    t_token *prev;
    int is_separator;

    if (!tokens)
    {
        fprintf(stderr, "[ERROR] No tokens to validate.\n");
        return 0;
    }

    /* Le premier token doit être un mot (TOKEN_WORD) */
    if (tokens->type != TOKEN_WORD)
    {
        fprintf(stderr, "[ERROR] Command starts with invalid token: '%s'\n", tokens->value);
        return 0;
    }

    segment_start = tokens;  // Début du premier segment
    prev = NULL;
    is_separator = 0;
    current = tokens;

    while (current)
    {
        /* Vérifie si un token non supporté est présent */
        if (current->type == TOKEN_UNSUPPORTED)
        {
            fprintf(stderr, "[ERROR] Unsupported token detected: '%s'\n", current->value);
            return 0;
        }

        /* Si le token est un séparateur (PIPE, SEMICOLON, redirections, HEREDOC) */
        if (current->type == TOKEN_PIPE ||
            current->type == TOKEN_SEMICOLON ||
            current->type == TOKEN_REDIRECT_IN ||
            current->type == TOKEN_REDIRECT_OUT ||
            current->type == TOKEN_REDIRECT_APPEND ||
            current->type == TOKEN_HEREDOC)
        {
            if (is_separator)
            {
                fprintf(stderr, "[ERROR] Invalid sequence of tokens near: '%s'\n", current->value);
                return 0;
            }
            is_separator = 1;

            /* Pour un point-virgule, vérifie que le token suivant existe */
            if (current->type == TOKEN_SEMICOLON)
            {
                if (!current->next)
                {
                    fprintf(stderr, "[ERROR] Unexpected semicolon ';' (empty command).\n");
                    return 0;
                }
                /* Si le token suivant n'est pas un mot, c'est l'erreur attendue */
                if (current->next->type != TOKEN_WORD)
                {
                    fprintf(stderr, "[ERROR] Command starts with invalid token: '%s'\n", current->next->value);
                    return 0;
                }
                /* On passe au token suivant et on réinitialise segment_start */
                current = current->next;
                segment_start = current;
                is_separator = 0;
                continue;
            }

            /* Pour un heredoc, vérifier qu'un délimiteur (TOKEN_WORD) suit et qu'il est attaché à une commande */
            if (current->type == TOKEN_HEREDOC)
            {
                if (!current->next || current->next->type != TOKEN_WORD)
                {
                    fprintf(stderr, "[ERROR] Missing delimiter for heredoc ('<<').\n");
                    return 0;
                }
                if (!prev || prev->type != TOKEN_WORD)
                {
                    fprintf(stderr, "[ERROR] Unexpected heredoc '<<' (not attached to a command).\n");
                    return 0;
                }
                is_separator = 0;
            }
            /* Pour les autres séparateurs (pipe, redirections), vérifier qu'un TOKEN_WORD suit immédiatement */
            else if (!current->next || current->next->type != TOKEN_WORD)
            {
                if (current->type == TOKEN_PIPE)
                    fprintf(stderr, "[ERROR] Command ends with a pipe '|' (incomplete command).\n");
                else
                    fprintf(stderr, "[ERROR] Missing file for redirection.\n");
                return 0;
            }
        }
        else
        {
            is_separator = 0;
        }

        prev = current;
        current = current->next;
    }

    /* Vérifie que le dernier segment est non vide et commence par un mot */
    if (segment_start && segment_start->type != TOKEN_WORD)
    {
        fprintf(stderr, "[ERROR] Unexpected trailing separator (empty command).\n");
        return 0;
    }
    return 1;
}

int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	handle_special_char(const char *input, t_token **tokens, int i)
{
	if (input[i + 1] == input[i])
	{
		if (input[i] == '<')
			token_add(tokens, token_new(ft_substr(input, i, 2), TOKEN_REDIRECT_IN, NO_QUOTE));
		else
			token_add(tokens, token_new(ft_substr(input, i, 2), TOKEN_REDIRECT_OUT, NO_QUOTE));
		return (i + 2);
	}
	if (input[i] == '|')
		token_add(tokens, token_new(ft_substr(input, i, 1), TOKEN_PIPE, NO_QUOTE));
	else
		token_add(tokens, token_new(ft_substr(input, i, 1), TOKEN_REDIRECT_OUT, NO_QUOTE));
	return (i + 1);
}

/* Initialise une nouvelle structure t_command */
t_command	*init_new_command(t_shell *shell)
{
	t_command	*cmd;

	cmd = safe_malloc(sizeof(t_command), shell);
	if (!cmd)
		return (NULL);

	/* Tout initialiser proprement */
	cmd->argv = NULL;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->append = 0;
	cmd->heredoc = 0; 
	cmd->heredoc_delimiter = NULL;
	cmd->pipe_in = -1;
	cmd->pipe_out = 0;
	cmd->next = NULL;
	return (cmd);
}
/* 📌 Amélioration de organize_commands() avec le support de `;` */
t_command	*organize_commands(t_token *tokens, t_shell *shell)
{
	t_command	*commands;
	t_command	*current;
	t_command	*tmp;

	commands = NULL;
	current = NULL;

	while (tokens)
	{
		// Si on rencontre `;`, on démarre une nouvelle commande après
		if (tokens->type == TOKEN_SEMICOLON)
		{
			current = NULL; // Séparation nette des commandes
			tokens = tokens->next;
			continue;
		}

		// Initialiser une nouvelle commande si aucune n'existe
		if (!current)
		{
			current = init_new_command(shell);
			if (!current)
			{
				print_error("organize_commands", "memory allocation failed");
				free_commands(commands);
				return (NULL);
			}
			if (!commands)
				commands = current;
			else
			{
				tmp = commands;
				while (tmp->next)
					tmp = tmp->next;
				tmp->next = current;
			}
		}

		// Gestion des tokens
		if (tokens->type == TOKEN_WORD)
		{
			char *arg = safe_strdup(tokens->value, shell);
			if (!arg)
			{
				print_error("organize_commands", "memory allocation failed for argument");
				free_commands(commands);
				return (NULL);
			}
			add_argument_to_command(current, arg, shell);
		}
		else if (tokens->type == TOKEN_PIPE)
		{
			current->pipe_out = 1;
			tokens = tokens->next;
			if (tokens)
			{
				current->next = init_new_command(shell);
				if (!current->next)
				{
					print_error("organize_commands", "memory allocation failed after pipe");
					free_commands(commands);
					return (NULL);
				}
				current->next->pipe_in = 1;
				current = current->next;
			}
			continue ;
		}
		else if (tokens->type == TOKEN_REDIRECT_IN && tokens->next && tokens->next->type == TOKEN_WORD)
		{
			if (!tokens->next->value)
			{
				print_error("organize_commands", "missing file for input redirection '<'");
				free_commands(commands);
				return (NULL);
			}
			current->input_file = safe_strdup(tokens->next->value, shell);
			tokens = tokens->next;
		}
		else if ((tokens->type == TOKEN_REDIRECT_OUT || tokens->type == TOKEN_REDIRECT_APPEND)
			&& tokens->next && tokens->next->type == TOKEN_WORD)
		{
			if (!tokens->next->value)
			{
				print_error("organize_commands", "missing file for output redirection '>'");
				free_commands(commands);
				return (NULL);
			}
			current->output_file = safe_strdup(tokens->next->value, shell);
			current->append = (tokens->type == TOKEN_REDIRECT_APPEND);
			tokens = tokens->next;
		}
		else if (tokens->type == TOKEN_HEREDOC && tokens->next && tokens->next->type == TOKEN_WORD)
		{
			if (!tokens->next->value)
			{
				print_error("organize_commands", "missing delimiter for heredoc");
				free_commands(commands);
				return (NULL);
			}
			current->heredoc_delimiter = safe_strdup(tokens->next->value, shell);
			current->heredoc = 1;
			tokens = tokens->next;
		}
		else if (tokens->type == TOKEN_UNSUPPORTED)
		{
			print_error("organize_commands", "unsupported token detected");
			free_commands(commands);
			return (NULL);
		}

		tokens = tokens->next;
	}

	if (!commands)
	{
		print_error("organize_commands", "no valid commands detected");
		return (NULL);
	}

	return (commands);
}
