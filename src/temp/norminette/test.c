#include "minishell.h"

/* Trouve l'index de fermeture des quotes */

int	get_end_of_quotes(const char *str, int start)
{
	char	quote = str[start];
	int		i = start + 1;

	while (str[i] && str[i] != quote)
		i++;
	if (str[i] == '\0') // Quotes non fermées
		return (-1);
	return (i + 1);
}
/* Trouve la fin d'un argument, y compris les quotes */
int	get_end_of_argument(const char *str, int start)
{
	int	i = start;

	while (str[i] && str[i] != ' ' && str[i] != '|' && str[i] != '<' && str[i] != '>')
		i++;
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
/* Ajoute un argument comme token après traitement des quotes et expansions */
void	add_argument_token(const char *input, int start, int end, t_token **tokens, t_env_var *env)
{
	char	*raw_value;
	char	*processed_value;

	// Extraire la sous-chaîne de l'argument brut
	raw_value = ft_substr(input, start, end - start);
	if (!raw_value)
		return ; // Gestion simplifiée en cas d'erreur de mémoire

	// Traiter les quotes et l'expansion des variables
	processed_value = process_quotes_and_expansion(raw_value, env);
	free(raw_value); // Libérer la mémoire de la chaîne brute
	if (!processed_value)
		return ; // Gestion d'erreur si `process_quotes_and_expansion` échoue

	// Ajouter le token à la liste
	token_add(tokens, token_new(processed_value, TOKEN_WORD, NO_QUOTE));
	// La mémoire de `processed_value` est transférée dans le token, pas besoin de free ici
}

