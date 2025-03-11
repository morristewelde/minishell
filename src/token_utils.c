/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 15:52:06 by sruzzi            #+#    #+#             */
/*   Updated: 2025/03/07 15:52:14 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

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

	if (!list || !*list) // ✅ Vérifie si `list` est NULL ou vide
		return ;

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
		return ;
	if (!*list)
	{
		*list = new_token;
		return ;
	}
	current = *list;
	while (current->next)
		current = current->next;
	current->next = new_token;
	//printf("[DEBUG] token_add: Added token with value '%s'\n", new_token->value);
}
t_token *get_last_token(t_token *tokens)
{
	if (!tokens)
		return (NULL);

	while (tokens->next)
		tokens = tokens->next;

	return (tokens);
}
