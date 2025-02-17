#include "../include/minishell.h"

t_token	*token_new(char *value, t_token_type type, t_quote_type quote)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->value = ft_strdup(value);
	new_token->type = type;
	new_token->quote = quote;
	new_token->next = NULL;
	return (new_token);
}

void	token_free(t_token **list)
{
	t_token	*temp;

	while (*list)
	{
		temp = (*list)->next;
		free((*list)->value);
		free(*list);
		*list = temp;
	}
}

/* Crée un nouveau token */
/* Ajoute un token à la liste */
void	token_add(t_token **list, t_token *new_token)
{
	t_token	*current;

	if (!list || !new_token)
		return;
	if (!*list)
	{
		*list = new_token;
		return;
	}
	current = *list;
	while (current->next)
		current = current->next;
	current->next = new_token;
	//printf("[DEBUG] token_add: Added token with value '%s'\n", new_token->value);
}
#include "minishell.h"

/* Libère un token et une valeur en cas d'erreur */
static t_token	*free_token_and_value(char *value, t_token **tokens)
{
	if (value)
		free(value);
	if (tokens)
		token_free(tokens);
	return (NULL); // ✅ Retourne NULL pour éviter l'erreur de compilation
}

/* Tokenize l'entrée utilisateur */
t_token	*tokenize_input(const char *input, t_shell *shell)
{
	t_token			*tokens;
	int				i;
	int				start;
	t_token_type	type;
	char			*processed_value;
	char			*raw_value;

	tokens = NULL;
	i = 0;
	if (!check_quotes(input))
		return (NULL);

	if (ft_strcmp(input, ":") == 0 || ft_strcmp(input, "!") == 0)
        return NULL; // Return NULL to indicate no tokens
	while (input[i])
	{
		i = get_next_index(input, i);
		if (!input[i])
			break ;

		/* Gestion des tokens `&&` ou `||` (non supportés) */
		if ((input[i] == '&' && input[i + 1] == '&') ||
			(input[i] == '|' && input[i + 1] == '|'))
		{
			raw_value = ft_substr(input, i, 2);
			if (!raw_value)
				return (free_token_and_value(NULL, &tokens));
			token_add(&tokens, token_new(raw_value, TOKEN_UNSUPPORTED, NO_QUOTE));
			i += 2;
			continue ;
		}

		/* Gestion des pipes `|` */
		if (input[i] == '|')
		{
			raw_value = ft_substr(input, i, 1);
			if (!raw_value)
				return (free_token_and_value(NULL, &tokens));
			token_add(&tokens, token_new(raw_value, TOKEN_PIPE, NO_QUOTE));
			i++;
			continue ;
		}

		/* 📌 Ajout du point-virgule `;` */
		if (input[i] == ';')
		{
			raw_value = ft_substr(input, i, 1);
			if (!raw_value)
				return (free_token_and_value(NULL, &tokens));
			token_add(&tokens, token_new(raw_value, TOKEN_SEMICOLON, NO_QUOTE));
			i++;
			continue ;
		}

		/* Gestion des redirections `<`, `>`, `<<`, `>>` */
		if (input[i] == '<' || input[i] == '>')
		{
			if (input[i + 1] == input[i]) // Cas heredoc `<<` ou append `>>`
			{
				type = (input[i] == '<') ? TOKEN_HEREDOC : TOKEN_REDIRECT_APPEND;
				raw_value = ft_substr(input, i, 2);
				if (!raw_value)
					return (free_token_and_value(NULL, &tokens));
				token_add(&tokens, token_new(raw_value, type, NO_QUOTE));
				i += 2;

				/* Si heredoc `<<`, lire le délimiteur brut */
				if (type == TOKEN_HEREDOC)
				{
					i = get_next_index(input, i);
					start = i;
					if (input[i] == '\'' || input[i] == '"')
						i = get_end_of_quotes(input, i);
					else
						i = get_end_of_argument(input, i);
					if (i == -1)
						return (free_token_and_value(NULL, &tokens));
					raw_value = ft_substr(input, start, i - start);
					if (!raw_value)
						return (free_token_and_value(NULL, &tokens));
					token_add(&tokens, token_new(raw_value, TOKEN_WORD, NO_QUOTE));
				}
				continue ;
			}
			else // Cas simple `<` ou `>`
			{
				type = (input[i] == '<') ? TOKEN_REDIRECT_IN : TOKEN_REDIRECT_OUT;
				raw_value = ft_substr(input, i, 1);
				if (!raw_value)
					return (free_token_and_value(NULL, &tokens));
				token_add(&tokens, token_new(raw_value, type, NO_QUOTE));
				i++;
				continue ;
			}
		}

		/* Gestion des arguments classiques */
		start = i;
		if (input[i] == '\'' || input[i] == '"')
			i = get_end_of_quotes(input, i);
		else
			i = get_end_of_argument(input, i);
		if (i == -1)
			return (free_token_and_value(NULL, &tokens));

		raw_value = ft_substr(input, start, i - start);
		if (!raw_value)
			return (free_token_and_value(NULL, &tokens));

		processed_value = process_quotes_and_expansion(raw_value, shell);
		if (!processed_value)
			return (free_token_and_value(raw_value, &tokens));

		free(raw_value);
		token_add(&tokens, token_new(processed_value, TOKEN_WORD, NO_QUOTE));
	}
	return (tokens);
}

