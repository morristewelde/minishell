/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 15:48:34 by sruzzi            #+#    #+#             */
/*   Updated: 2025/03/07 15:48:43 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

/*
** Modifie l'état des quotes.
** - `c`: caractère actuel à analyser.
** - `current_quote`: état actuel des quotes (NO_QUOTE, SINGLE_QUOTE, DOUBLE_QUOTE).
** Retourne le nouvel état des quotes.
*/
t_quote_type	set_in_quotes(char c, t_quote_type current_quote)
{
	if (current_quote == NO_QUOTE)
	{
		if (c == '\'')
			return (SINGLE_QUOTE);
		else if (c == '"')
			return (DOUBLE_QUOTE);
	}
	else if ((current_quote == SINGLE_QUOTE && c == '\'') ||
			(current_quote == DOUBLE_QUOTE && c == '"'))
		return (NO_QUOTE);
	return (current_quote);
}

/*
** Vérifie si toutes les quotes dans la chaîne sont fermées.
** - `input`: chaîne à analyser.
** Retourne 1 si les quotes sont fermées, 0 sinon.
*/
int	check_quotes(const char *input)
{
	t_quote_type	quote_state;
	int				i;

	quote_state = NO_QUOTE;
	i = 0;
	while (input[i])
	{
		quote_state = set_in_quotes(input[i], quote_state);
		i++;
	}
	if (quote_state != NO_QUOTE)
	{
		write(2, "Error: unclosed quotes\n", 23);
		return (0);
	}
	return (1);
}
