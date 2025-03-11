#include "minishell.h"

/* Vérifie que la commande ne commence pas par un PIPE et qu'aucun token n'est UNSUPPORTED */
static int	check_initial_tokens(t_token *tokens)
{
	t_token	*current;

	if (tokens && tokens->type == TOKEN_PIPE)
	{
		fprintf(stderr, "Error: command starts with a pipe\n");
		return (0);
	}
	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_UNSUPPORTED)
		{
			fprintf(stderr, "Error: unsupported token detected\n");
			return (0);
		}
		current = current->next;
	}
	return (1);
}

/* Vérifie si un token est un séparateur (PIPE, redirections ou HEREDOC) */
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
	return (0);
}

/* Gère les erreurs liées aux tokens séparateurs */
static int	handle_separator_errors(t_token *current, int is_separator)
{
	if (is_separator)
	{
		fprintf(stderr, "Error: invalid sequence of tokens\n");
		return (0);
	}
	if (!current->next || current->next->type != TOKEN_WORD)
	{
		if (current->type == TOKEN_PIPE)
		{
			fprintf(stderr, "Error: command ends with a pipe\n");
		}
		else if (current->type == TOKEN_HEREDOC)
		{
			if (!current->next)
				fprintf(stderr, "Error: missing delimiter for heredoc\n");
			else
				return (1);
		}
		else
			fprintf(stderr, "Error: missing file for redirection\n");
		return (0);
	}
	return (1);
}

/* Vérifie la syntaxe particulière d'un heredoc */
static int	check_heredoc_syntax(t_token *current)
{
	if (current->type == TOKEN_HEREDOC && current->next &&
	    current->next->next && current->next->next->type == TOKEN_WORD)
	{
		fprintf(stderr, "Error: unexpected token after heredoc delimiter\n");
		return (0);
	}
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
			if (!check_heredoc_syntax(current))
				return (0);
			is_separator = 1;
		}
		else
		{
			is_separator = 0;
		}
		current = current->next;
	}
	return (1);
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
/*
** 1) add_command_if_needed
** Crée une nouvelle commande si *current est NULL et l'insère
** à la fin de la liste chaînée *commands.
*/
int	add_command_if_needed(t_command **commands, t_command **current, t_shell *shell)
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
** 4) handle_redirect_in_token
** Gestion du token REDIRECT_IN : copie du fichier si présent.
*/
int	handle_redirect_in_token(t_token **tokens, t_command *cur, t_shell *shell, t_command *cmds)
{
	if (!(*tokens)->next || (*tokens)->next->type != TOKEN_WORD || !(*tokens)->next->value)
	{
		print_error("organize_commands", "missing file for input redirection '<'");
		free_commands(cmds);
		return (0);
	}
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
** Gestion des tokens REDIRECT_OUT ou REDIRECT_APPEND.
*/
int	handle_redirect_out_token(t_token **tokens, t_command *cur, t_shell *shell, t_command *cmds)
{
	if (!(*tokens)->next || (*tokens)->next->type != TOKEN_WORD || !(*tokens)->next->value)
	{
		print_error("organize_commands", "missing file for output redirection '>'");
		free_commands(cmds);
		return (0);
	}
	cur->output_file = safe_strdup((*tokens)->next->value, shell);
	if (!cur->output_file)
	{
		print_error("organize_commands", "memory allocation failed for output_file");
		free_commands(cmds);
		return (0);
	}
	cur->append = ((*tokens)->type == TOKEN_REDIRECT_APPEND);
	(*tokens) = (*tokens)->next;
	return (1);
}

/*
** 6) handle_heredoc_token
** Gestion du token HEREDOC.
*/
int	handle_heredoc_token(t_token **tokens, t_command *cur, t_shell *shell, t_command *cmds)
{
	if (!(*tokens)->next || (*tokens)->next->type != TOKEN_WORD || !(*tokens)->next->value)
	{
		print_error("organize_commands", "missing delimiter for heredoc");
		free_commands(cmds);
		return (0);
	}
	cur->heredoc_delimiter = safe_strdup((*tokens)->next->value, shell);
	if (!cur->heredoc_delimiter)
	{
		print_error("organize_commands", "memory allocation failed for heredoc_delimiter");
		free_commands(cmds);
		return (0);
	}
	cur->heredoc = 1;
	(*tokens) = (*tokens)->next;
	return (1);
}

/*
** 7) handle_unsupported_token
** Gestion du token UNSUPPORTED.
*/
int	handle_unsupported_token(t_command *cmds)
{
	print_error("organize_commands", "unsupported token detected");
	free_commands(cmds);
	return (0);
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
	else if ((*tokens)->type == TOKEN_UNSUPPORTED)
		return (handle_unsupported_token(*cmds));
	return (1);
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
