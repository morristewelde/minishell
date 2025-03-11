/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 16:28:13 by sruzzi            #+#    #+#             */
/*   Updated: 2025/03/07 16:28:20 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

/* Trouve l'index de fermeture des quotes */
int	get_end_of_quotes(const char *str, int start)
{
	char	quote;
	int		i;

	quote = str[start];
	i = start + 1; // Passer le premier guillemet
	while (str[i] && str[i] != quote)
		i++;

	if (!str[i]) // Si on atteint la fin sans trouver le guillemet fermant
		return (-1);

	return (i + 1); // Retourner après la fin du guillemet
}

/* Trouve la fin d'un argument, y compris les quotes */
int	get_end_of_argument(const char *str, int start)
{
	int				i;
	t_quote_type	quote_state;

	i = start;
	quote_state = NO_QUOTE;

	while (str[i])
	{
		// Gestion des quotes
		if (str[i] == '"' || str[i] == '\'')
		{
			if (quote_state == NO_QUOTE)
				quote_state = (str[i] == '"') ? DOUBLE_QUOTE : SINGLE_QUOTE;
			else if ((quote_state == DOUBLE_QUOTE && str[i] == '"') ||
					(quote_state == SINGLE_QUOTE && str[i] == '\''))
				quote_state = NO_QUOTE;
		}
		// Si on trouve un espace HORS quotes, on arrête
		else if (str[i] == ' ' && quote_state == NO_QUOTE)
			break;
		// Si on rencontre `|`, `<`, `>` HORS quotes, on arrête aussi
		else if ((str[i] == '|' || str[i] == '<' || str[i] == '>') && quote_state == NO_QUOTE)
			break;
		i++;
	}
	return (i);
}

/* Avance jusqu'au prochain argument ou token */
int	get_next_index(const char *str, int start)
{
	int	i;

	i = start;
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++; // Ignore les espaces
	return (i);
}

