/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
// Déclaration des structures nécessaires
typedef struct s_env_var {
	char *name;
	char *value;
	struct s_env_var *next;
} t_env_var;

// Déclaration des constantes pour les types de quotes
#define NO_QUOTE 0
#define SINGLE_QUOTE 1
#define DOUBLE_QUOTE 2

// Prototypes des fonctions nécessaires
char	*ft_strjoin(const char *s1, const char *s2);
char	*replace_env_variables(const char *input, t_env_var *env);
char	*replace_variable(const char *input, int *i, char *result, t_env_var *env);
char	*ft_substr(const char *s, unsigned int start, size_t len);
char	*ft_strdup(const char *s);
t_env_var	*env_find_var(t_env_var *env, const char *name);
int		handle_quotes_segment(const char *input, int i, t_env_var *env, char **formatted_input, int *quote_state);
int		ft_isalnum(char c)
{
	return (isalnum(c));
}
char *ft_strjoin_free(char *s1, char *s2) {
    char *result = ft_strjoin(s1, s2);
    free(s1);
    return result;
}
char	*append_char(char *result, char c)
{
	char	temp[2];
	char	*new_result;

	temp[0] = c;
	temp[1] = '\0';
	new_result = ft_strjoin(result, temp);
	free(result);
	if (!new_result)
		printf("[DEBUG] append_char: Failed to append character\n");
	return (new_result);
}


// Fonction de debug : afficher une erreur claire
void debug_log(const char *context, const char *message) {
    fprintf(stderr, "[DEBUG] %s: %s\n", context, message);
}

// Ajout des détails des fonctions manquantes pour le parsing ici...
// Exemple d'une fonction existante modifiée avec du debug :
char *replace_variable(const char *input, int *i, char *result, t_env_var *env) {
    int var_start = ++(*i);
    char *var_name;
    char *var_value;
    char *temp;

    while (ft_isalnum(input[*i]) || input[*i] == '_')
        (*i)++;
    
    var_name = ft_substr(input, var_start, *i - var_start);
    if (!var_name) {
        debug_log("replace_variable", "Failed to allocate var_name");
        return NULL;
    }

    var_value = env_find_var(env, var_name) ? ft_strdup(env_find_var(env, var_name)->value) : ft_strdup("");
    free(var_name);
    if (!var_value) {
        debug_log("replace_variable", "Failed to allocate var_value");
        return NULL;
    }

    temp = ft_strjoin(result, var_value);
    free(result);
    free(var_value);
    if (!temp) {
        debug_log("replace_variable", "Failed to concatenate var_value to result");
        return NULL;
    }

    return temp;
}
char *parse_entire_line(const char *input, t_env_var *env)
{
	int i = 0;
	char *formatted = strdup(""); // Résultat final
	int quote_state = NO_QUOTE;

	if (!formatted)
		return NULL;

	while (input[i])
	{
		// Gestion des quotes simples ou doubles
		if (input[i] == '\'' || input[i] == '"')
		{
			int ret = handle_quotes_segment(input, i, env, &formatted, &quote_state);
			if (ret == -1)
			{
				free(formatted);
				return NULL; // ou afficher une erreur
			}
			i = ret; // Avancer après la quote fermante
		}
		// Gestion des variables d'environnement hors quotes simples
		else if (input[i] == '$' && quote_state != SINGLE_QUOTE)
		{
			int start = i;
			i++; // Passer le symbole '$'
			while (ft_isalnum(input[i]) || input[i] == '_')
				i++; // Identifier le nom de la variable
			char *var_name = ft_substr(input, start + 1, i - start - 1);
			char *var_value = replace_env_variables(var_name, env);
			if (!var_value)
				var_value = ft_strdup(""); // Variable non définie
			formatted = ft_strjoin_free(formatted, var_value);
			free(var_name);
		}
		// Gestion des caractères normaux (hors quotes et variables)
		else
		{
			formatted = append_char(formatted, input[i]);
			if (!formatted)
				return NULL;
			i++;
		}
	}
	return formatted;
}


char *replace_env_variables(const char *input, t_env_var *env)
{
	char *result;
	int i;

	printf("[DEBUG] Starting replace_env_variables with input: '%s'\n", input);

	result = malloc(1);
	if (!result)
	{
		fprintf(stderr, "[DEBUG] Error: failed to allocate memory for result\n");
		return NULL;
	}
	result[0] = '\0';
	i = 0;

	while (input[i])
	{
		// Gestion des variables d'environnement
		if (input[i] == '$')
		{
			if (ft_isalnum(input[i + 1]) || input[i + 1] == '_')
			{
				printf("[DEBUG] Found variable at index %d\n", i);
				result = replace_variable(input, &i, result, env);
				if (!result)
				{
					fprintf(stderr, "[DEBUG] Error: failed to replace variable at index %d\n", i);
					return NULL;
				}
			}
			else
			{
				result = append_char(result, input[i]);
				if (!result)
				{
					fprintf(stderr, "[DEBUG] Error: failed to append '$' at index %d\n", i);
					return NULL;
				}
				i++;
			}
		}
		else
		{
			result = append_char(result, input[i]);
			if (!result)
			{
				fprintf(stderr, "[DEBUG] Error: failed to append character '%c' at index %d\n", input[i], i);
				return NULL;
			}
			i++;
		}
	}

	printf("[DEBUG] Final replaced string: '%s'\n", result);
	return result;
}


char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*substr;

	if (!s)
		return (NULL);
	substr = malloc(len + 1);
	if (!substr)
		return (NULL);
	for (i = 0; i < len && s[start + i]; i++)
		substr[i] = s[start + i];
	substr[i] = '\0';
	return (substr);
}

char	*ft_strdup(const char *s)
{
	size_t	len;
	char	*dup;

	len = strlen(s);
	dup = malloc(len + 1);
	if (!dup)
		return (NULL);
	strcpy(dup, s);
	return (dup);
}

char	*ft_strjoin(const char *s1, const char *s2)
{
	size_t	len1;
	size_t	len2;
	char	*result;

	len1 = strlen(s1);
	len2 = strlen(s2);
	result = malloc(len1 + len2 + 1);
	if (!result)
		return (NULL);
	strcpy(result, s1);
	strcat(result, s2);
	return (result);
}

t_env_var	*env_new_var(const char *name, const char *value)
{
	t_env_var	*new_var;

	new_var = malloc(sizeof(t_env_var));
	if (!new_var)
		return (NULL);
	new_var->name = strdup(name);
	new_var->value = strdup(value);
	new_var->next = NULL;
	return (new_var);
}

void	env_add_var(t_env_var **env, t_env_var *new_var)
{
	t_env_var	*current;

	current = *env;
	if (!current)
		*env = new_var;
	else {
		while (current->next)
			current = current->next;
		current->next = new_var;
	}
}

t_env_var	*env_find_var(t_env_var *env, const char *name)
{
	while (env)
	{
		if (strcmp(env->name, name) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

int handle_quotes_segment(const char *input, int i, t_env_var *env, char **formatted_input, int *quote_state)
{
	char quote_char;
	char *quote_content;
	char *processed_content;
	int start;

	// Vérifie si input[i] est une quote ouvrante
	if (input[i] != '\'' && input[i] != '"')
		return i;

	quote_char = input[i]; // Quote ouvrante (' ou ")
	*quote_state = (quote_char == '\'') ? SINGLE_QUOTE : DOUBLE_QUOTE;
	start = ++i;

	// Parcourir jusqu'à trouver la quote fermante
	while (input[i] && input[i] != quote_char)
		i++;

	// Si la quote fermante est absente, erreur
	if (input[i] != quote_char)
	{
		write(2, "[DEBUG] Error: unclosed quotes starting at index %d\n", start - 1);
		return -1;
	}

	// Extraire le contenu entre les quotes
	quote_content = ft_substr(input, start, i - start);
	if (!quote_content)
		return -1;

	// Expansion si quote double
	if (*quote_state == DOUBLE_QUOTE)
		processed_content = replace_env_variables(quote_content, env);
	else
		processed_content = ft_strdup(quote_content);

	free(quote_content);
	if (!processed_content)
		return -1;

	// Ajouter le contenu traité au résultat
	char *temp = ft_strjoin(*formatted_input, processed_content);
	free(*formatted_input);
	free(processed_content);
	if (!temp)
		return -1;
	*formatted_input = temp;

	// Avancer après la quote fermante
	i++;
	*quote_state = NO_QUOTE;
	return i;
}

// Fonction principale de test avec debug pour analyser chaque cas
void test_handle_quotes_segment() {
    // Création de l'environnement
    t_env_var *env = NULL;
    env_add_var(&env, env_new_var("USER", "42student"));
    env_add_var(&env, env_new_var("HOME", "/home/42student"));
    env_add_var(&env, env_new_var("USER_HOME", "/home/42student"));

    printf("=== Tests avancés ===\n");

    // Test 7 : Variable non définie
    char *input7 = "Hello $UNDEFINED";
    char *formatted = strdup("");
    int quote_state = 0;
    int i = handle_quotes_segment(input7, 0, env, &formatted, &quote_state);
    printf("Test 7: %s (i = %d)\n", (i != -1) ? formatted : "Error", i);
    free(formatted);

    // Ajout d'autres tests avec debug...
    // Exemple : Test d'un cas complexe ou mal géré dans les tests précédents


    // Test 8 : Double $
    char *input8 = "Value: $$";
    formatted = strdup("");
    i = handle_quotes_segment(input8, 0, env, &formatted, &quote_state);
    printf("Test 8: %s (i = %d)\n", (i != -1) ? formatted : "Error", i);
    free(formatted);

    // Test 9 : Chaîne très longue
    char input9[1024] = "Hello $USER";
    for (int j = 0; j < 100; j++)
        strcat(input9, ", $USER");
    formatted = strdup("");
    i = handle_quotes_segment(input9, 0, env, &formatted, &quote_state);
    printf("Test 9: %s (truncated, length = %zu)\n", (i != -1) ? formatted : "Error", strlen(formatted));
    free(formatted);

    // Test 10 : Quotes imbriquées correctement formées
    char *input10 = "\"Hello 'world'\"";
    formatted = strdup("");
    i = handle_quotes_segment(input10, 0, env, &formatted, &quote_state);
    printf("Test 10: %s (i = %d)\n", (i != -1) ? formatted : "Error", i);
    free(formatted);

    // Test 11 : Quotes mal imbriquées
    char *input11 = "\"Hello 'world";
    formatted = strdup("");
    i = handle_quotes_segment(input11, 0, env, &formatted, &quote_state);
    printf("Test 11: %s (i = %d)\n", (i != -1) ? formatted : "Error", i);
    free(formatted);

    // Test 12 : Variable avec underscore et chiffres
    char *input12 = "Welcome to $USER_HOME";
    formatted = strdup("");
    i = handle_quotes_segment(input12, 0, env, &formatted, &quote_state);
    printf("Test 12: %s (i = %d)\n", (i != -1) ? formatted : "Error", i);
    free(formatted);

    // Libération
    while (env)
    {
        t_env_var *temp = env->next;
        free(env->name);
        free(env->value);
        free(env);
        env = temp;
    }
}

int main(void)
{
    test_handle_quotes_segment();
    return 0;
}
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Déclaration des structures nécessaires
typedef struct s_env_var {
    char *name;
    char *value;
    struct s_env_var *next;
} t_env_var;

// Déclaration des constantes pour les types de quotes
#define NO_QUOTE 0
#define SINGLE_QUOTE 1
#define DOUBLE_QUOTE 2

// Prototypes des fonctions nécessaires
char *append_char(char *result, char c);
char *replace_env_variables(const char *input, t_env_var *env);
char *ft_strjoin(const char *s1, const char *s2);
char *replace_variable(const char *input, int *i, char *result, t_env_var *env);
char *ft_substr(const char *s, unsigned int start, size_t len);
char *ft_strdup(const char *s);
t_env_var *env_find_var(t_env_var *env, const char *name);
int handle_quotes_segment(const char *input, int i, t_env_var *env, char **formatted_input, int *quote_state);

// Fonctions utilitaires
char *ft_substr(const char *s, unsigned int start, size_t len) {
    size_t i;
    char *substr;

    if (!s)
        return NULL;
    substr = malloc(len + 1);
    if (!substr)
        return NULL;
    for (i = 0; i < len && s[start + i]; i++)
        substr[i] = s[start + i];
    substr[i] = '\0';
    return substr;
}

char *ft_strdup(const char *s) {
    size_t len = strlen(s);
    char *dup = malloc(len + 1);
    if (!dup)
        return NULL;
    strcpy(dup, s);
    return dup;
}

char *ft_strjoin(const char *s1, const char *s2) {
    size_t len1, len2;
    char *result;

    len1 = strlen(s1);
    len2 = strlen(s2);
    result = malloc(len1 + len2 + 1);
    if (!result)
        return NULL;
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

t_env_var *env_new_var(const char *name, const char *value) {
    t_env_var *new_var = malloc(sizeof(t_env_var));
    if (!new_var)
        return NULL;
    new_var->name = ft_strdup(name);
    new_var->value = ft_strdup(value);
    new_var->next = NULL;
    return new_var;
}

void env_add_var(t_env_var **env, t_env_var *new_var) {
    t_env_var *current = *env;
    if (!current)
        *env = new_var;
    else {
        while (current->next)
            current = current->next;
        current->next = new_var;
    }
}

t_env_var *env_find_var(t_env_var *env, const char *name) {
    while (env) {
        if (strcmp(env->name, name) == 0)
            return env;
        env = env->next;
    }
    return NULL;
}

char *append_char(char *result, char c) {
    char temp[2] = {c, '\0'};
    char *new_result = ft_strjoin(result, temp);
    free(result);
    return new_result;
}

// Fonction pour gérer les quotes avec `quote_state`
int handle_quotes_segment(const char *input, int i, t_env_var *env, char **formatted_input, int *quote_state) {
    char quote_char = input[i]; // Déterminer la quote (simple ou double)
    char *quote_content, *processed_content;
    int start = ++i;

    *quote_state = (quote_char == '\'') ? SINGLE_QUOTE : DOUBLE_QUOTE;

    // Initialiser un buffer temporaire
    quote_content = malloc(strlen(input) + 1);
    if (!quote_content) {
        fprintf(stderr, "[DEBUG] Memory allocation error\n");
        return -1;
    }

    int content_index = 0;
    while (input[i]) {
        if (quote_char == '"' && input[i] == '\\' && input[i + 1]) {
            // Gestion des backslashes dans les quotes doubles
            quote_content[content_index++] = input[++i];
        } else if (input[i] == quote_char) {
            // Quote fermante trouvée
            break;
        } else {
            // Ajouter le caractère au contenu
            quote_content[content_index++] = input[i];
        }
        i++;
    }

    // Si la quote fermante est absente
    if (input[i] != quote_char) {
        free(quote_content);
        fprintf(stderr, "[DEBUG] Error: Unclosed quote starting at index %d\n", start - 1);
        return -1;
    }

    // Terminer la chaîne
    quote_content[content_index] = '\0';

    // Traiter le contenu des quotes
    if (*quote_state == DOUBLE_QUOTE) {
        processed_content = replace_env_variables(quote_content, env);
    } else {
        processed_content = ft_strdup(quote_content);
    }

    free(quote_content);
    if (!processed_content) {
        fprintf(stderr, "[DEBUG] Error processing quote content\n");
        return -1;
    }

    // Ajouter le contenu traité au résultat
    char *temp = ft_strjoin(*formatted_input, processed_content);
    free(*formatted_input);
    free(processed_content);
    *formatted_input = temp;

    *quote_state = NO_QUOTE;
    return i + 1;
}

// Fonction principale pour parser la ligne
char *parse_entire_line(const char *input, t_env_var *env) {
    char *formatted = ft_strdup("");
    int i = 0, quote_state = NO_QUOTE;

    if (!formatted)
        return NULL;

    while (input[i]) {
        if (input[i] == '\'' || input[i] == '"') {
            i = handle_quotes_segment(input, i, env, &formatted, &quote_state);
            if (i == -1) {
                free(formatted);
                return NULL;
            }
        } else if (input[i] == '$' && quote_state != SINGLE_QUOTE) {
            formatted = replace_variable(input, &i, formatted, env);
            if (!formatted)
                return NULL;
        } else if (input[i] == '\\' && quote_state == DOUBLE_QUOTE && input[i + 1]) {
            // Gestion des échappements dans les quotes doubles
            formatted = append_char(formatted, input[++i]);
            i++;
        } else {
            formatted = append_char(formatted, input[i]);
            i++;
        }
    }

    return formatted;
}

char *replace_env_variables(const char *input, t_env_var *env) {
    char *result = malloc(1);
    int i = 0;

    if (!result) {
        fprintf(stderr, "[DEBUG] Error: Failed to allocate memory for result\n");
        return NULL;
    }
    result[0] = '\0';

    while (input[i]) {
        if (input[i] == '$' && (isalnum(input[i + 1]) || input[i + 1] == '_')) {
            result = replace_variable(input, &i, result, env);
            if (!result)
                return NULL;
        } else {
            result = append_char(result, input[i]);
            if (!result)
                return NULL;
            i++;
        }
    }
    return result;
}

// Fonction pour remplacer une variable
char *replace_variable(const char *input, int *i, char *result, t_env_var *env) {
    int var_start = ++(*i);
    char *var_name, *var_value, *temp;

    while (isalnum(input[*i]) || input[*i] == '_')
        (*i)++;

    var_name = ft_substr(input, var_start, *i - var_start);
    if (!var_name)
        return NULL;

    t_env_var *env_var = env_find_var(env, var_name);
    var_value = env_var ? ft_strdup(env_var->value) : ft_strdup("");

    free(var_name);
    if (!var_value)
        return NULL;

    temp = ft_strjoin(result, var_value);
    free(result);
    free(var_value);
    return temp;
}

// Tests
void test_parse_entire_line() {
    t_env_var *env = NULL;
    env_add_var(&env, env_new_var("USER", "42student"));
    env_add_var(&env, env_new_var("HOME", "/home/42student"));

    char *inputs[] = {
        "Hello 'World'",                     // Test 1: Simple quote
        "Welcome $USER",                     // Test 2: Simple variable expansion
        "'$USER'",                           // Test 3: Quote disables expansion
        "\"$USER\"",                         // Test 4: Double quote enables expansion
        "Undefined $NOT_SET",                // Test 5: Undefined variable
        "echo \"complex 'quoted string' with \\\"multiple\\\" levels\"", // Test 6
        "$USER is logged in",                // Test 7: Variable expansion in sentence
        "Escaped \\\"quote\\\" test"         // Test 8: Escaped quotes
    };

    for (int i = 0; i < 8; i++) {
        char *result = parse_entire_line(inputs[i], env);
        printf("Test %d: %s\n", i + 1, result ? result : "(null)");
        free(result);
    }

    // Libérer la mémoire de l'environnement
    while (env) {
        t_env_var *temp = env->next;
        free(env->name);
        free(env->value);
        free(env);
        env = temp;
    }
}

int main() {
    test_parse_entire_line();
    return 0;
}

