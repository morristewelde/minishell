#include <stdio.h>
#include <stdlib.h>
#include "minishell.h"

// Utils
char *ft_strdup(const char *s) {
    size_t len = strlen(s);
    char *dup = malloc(len + 1);
    if (!dup) {
        fprintf(stderr, "[DEBUG] Memory allocation failed in ft_strdup\n");
        return NULL;
    }
    strcpy(dup, s);
    return dup;
}

int is_redirection(char c) {
    return (c == '>' || c == '<');
}

char *ft_strjoin(const char *s1, const char *s2) {
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    char *result = malloc(len1 + len2 + 1);
    if (!result) {
        fprintf(stderr, "[DEBUG] Memory allocation failed in ft_strjoin\n");
        return NULL;
    }
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

char *ft_substr(const char *s, unsigned int start, size_t len) {
    size_t i;
    char *substr;

    if (!s) return NULL;
    substr = malloc(len + 1);
    if (!substr) {
        fprintf(stderr, "[DEBUG] Memory allocation failed in ft_substr\n");
        return NULL;
    }
    for (i = 0; i < len && s[start + i]; i++) substr[i] = s[start + i];
    substr[i] = '\0';
    return substr;
}

char *append_char(char *result, char c) {
    char temp[2] = {c, '\0'};
    char *new_result = ft_strjoin(result, temp);
    free(result);
    if (!new_result) {
        fprintf(stderr, "[DEBUG] Memory allocation failed in append_char\n");
    }
    return new_result;
}

// Gestion de l'environnement
t_env_var *env_new_var(const char *name, const char *value) {
    t_env_var *new_var = malloc(sizeof(t_env_var));
    if (!new_var) {
        fprintf(stderr, "[DEBUG] Memory allocation failed in env_new_var\n");
        return NULL;
    }
    new_var->name = ft_strdup(name);
    new_var->value = ft_strdup(value);
    new_var->next = NULL;
    return new_var;
}

void env_add_var(t_env_var **env, t_env_var *new_var) {
    t_env_var *current = *env;
    if (!current) {
        *env = new_var;
    } else {
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

// Gestion des segments
char *replace_env_variables(const char *input, t_env_var *env) {
    char *result = ft_strdup("");
    int i = 0;

    if (!result) return NULL;

    while (input[i]) {
        if (input[i] == '$' && isalnum(input[i + 1])) {
            int start = ++i;
            while (isalnum(input[i]) || input[i] == '_')
                i++;
            char *var_name = ft_substr(input, start, i - start);
            if (!var_name) {
                free(result);
                return NULL;
            }
            t_env_var *env_var = env_find_var(env, var_name);
            char *var_value = env_var ? ft_strdup(env_var->value) : ft_strdup("");
            free(var_name);
            if (!var_value) {
                free(result);
                return NULL;
            }

            // 🔴 AJOUT DEBUG ICI 🔴
            printf("[DEBUG] Replacing \"$%s\" with \"%s\"\n", var_name, var_value);

            char *temp = ft_strjoin(result, var_value);
            free(result);
            free(var_value);
            result = temp;
        } else {
            result = append_char(result, input[i]);
            i++;
        }
    }

    printf("[DEBUG] Final replaced string: \"%s\"\n", result);
    return result;
}


// Gère le backslash et affiche les logs
// Pseudo-code simplifié
int handle_backslash_segment(const char *input, int i, char **formatted_input, int quote_state)
{
    printf("[DEBUG] Backslash detected at index %d, Quote state: %d, Next char: %c\n", i, quote_state, input[i + 1]);

    if (quote_state == SINGLE_QUOTE)
    {
        // En quotes simples, on garde le backslash tel quel
        printf("[DEBUG] Inside single quotes, keeping backslash\n");
        *formatted_input = append_char(*formatted_input, input[i]);
    }
    else if (quote_state == DOUBLE_QUOTE)
    {
        if (input[i + 1] == '\\' || input[i + 1] == '$' || input[i + 1] == '"')
        {
            printf("[DEBUG] Inside double quotes, removing backslash before %c\n", input[i + 1]);
            i++; // Ignore le backslash
            *formatted_input = append_char(*formatted_input, input[i]); // Ajoute directement le caractère échappé
            return i + 1; // Passe au suivant
        }
        else
        {
            *formatted_input = append_char(*formatted_input, input[i]);
        }
    }
    else // Hors quotes
    {
        if (input[i + 1] == '\\' || input[i + 1] == '$' || input[i + 1] == '"' || input[i + 1] == '\'')
        {
            printf("[DEBUG] Outside quotes, skipping backslash before special char %c\n", input[i + 1]);
            i++; // Ignore le backslash
            *formatted_input = append_char(*formatted_input, input[i]); // Ajoute directement le caractère suivant
            return i + 1;
        }
        else if (!isspace(input[i + 1]) && input[i + 1] != '\0')
        {
            printf("[DEBUG] Outside quotes, ignoring backslash before normal char %c\n", input[i + 1]);
            i++;
            *formatted_input = append_char(*formatted_input, input[i]);
            return i + 1;
        }
        else
        {
            printf("[DEBUG] Outside quotes, keeping standalone backslash\n");
            *formatted_input = append_char(*formatted_input, input[i]);
        }
    }

    return i + 1;
}


int handle_quotes_segment(const char *input, int i, t_env_var *env, char **formatted_input, int *quote_state)
{
    char quote_char = input[i];
    int start = ++i;
    *quote_state = (quote_char == '\'') ? SINGLE_QUOTE : DOUBLE_QUOTE;

    while (input[i] && input[i] != quote_char)
    {
        if (quote_char == '"' && input[i] == '\\' && (input[i + 1] == '$' || input[i + 1] == '"' || input[i + 1] == '\\'))
        {
            printf("[DEBUG] Inside double quotes, skipping backslash before %c\n", input[i + 1]);
            i++;
        }
        i++;
    }

    if (input[i] != quote_char)
    {
        fprintf(stderr, "[ERROR] Unclosed quote starting at index %d\n", start - 1);
        return -1;
    }

    char *quote_content = ft_substr(input, start, i - start);
    if (!quote_content)
        return -1;

    char *processed_content = (*quote_state == DOUBLE_QUOTE) ? replace_env_variables(quote_content, env) : ft_strdup(quote_content);

    free(quote_content);
    if (!processed_content)
        return -1;

    *formatted_input = ft_strjoin(*formatted_input, processed_content);
    free(processed_content);

    *quote_state = NO_QUOTE;
    return i + 1;
}

int handle_expansion_segment(const char *input, int i, t_env_var *env, char **formatted_input)
{
    int var_start = ++i; // Passer après le '$'
    while (isalnum(input[i]) || input[i] == '_') // Nom de variable valide
        i++;

    char *var_name = ft_substr(input, var_start, i - var_start);
    if (!var_name)
        return -1;

    t_env_var *env_var = env_find_var(env, var_name);
    char *var_value = env_var ? ft_strdup(env_var->value) : ft_strdup("");
    free(var_name);
    if (!var_value)
        return -1;

    *formatted_input = ft_strjoin(*formatted_input, var_value);
    free(var_value);

    return i;
}

int handle_redirection_segment(const char *input, int i, char **formatted_input)
{
    *formatted_input = append_char(*formatted_input, input[i]);

    // Gestion des redirections doubles (>> ou <<)
    if ((input[i] == '>' && input[i + 1] == '>') || (input[i] == '<' && input[i + 1] == '<'))
    {
        i++; // Avancer pour inclure le deuxième caractère
        *formatted_input = append_char(*formatted_input, input[i]);
    }

    return i + 1; // Avancer après la redirection
}

int handle_pipe_segment(const char *input, int i, char **formatted_input)
{
    if (i == 0 || input[i + 1] == '|') // Vérification de syntaxe
    {
        fprintf(stderr, "[ERROR] Syntax error near unexpected token `|`\n");
        return -1;
    }

    *formatted_input = append_char(*formatted_input, '|');
    return i + 1; // Avancer après le pipe
}

int handle_word_segment(const char *input, int i, char **formatted_input, t_env_var *env)
{
    (void)env;

    while (input[i] && !isspace(input[i]) && input[i] != '|' && !is_redirection(input[i])
           && input[i] != '\'' && input[i] != '"')
    {
        if (i > 0 && input[i - 1] == '\\' && isspace(input[i]))
        {
            printf("[DEBUG] Skipping escaped space at index %d\n", i);
            *formatted_input = append_char(*formatted_input, input[i]);
        }
        else if (input[i] != '\\')
        {
            *formatted_input = append_char(*formatted_input, input[i]);
        }
        i++;
    }

    if (isspace(input[i]))
    {
        *formatted_input = append_char(*formatted_input, ' ');
        i++;
    }

    return i;
}

// Fonction principale avec logs globaux
char *format_input(const char *input, t_env_var *env)
{
    char *formatted_input = ft_strdup("");
    int i = 0;
    int quote_state = NO_QUOTE;

    if (!formatted_input)
        return NULL;

    while (input[i])
    {
        if (input[i] == '\'' || input[i] == '"')
            i = handle_quotes_segment(input, i, env, &formatted_input, &quote_state);
        else if (input[i] == '\\')
            i = handle_backslash_segment(input, i, &formatted_input, quote_state);
        else if (input[i] == '$' && quote_state != SINGLE_QUOTE)
            i = handle_expansion_segment(input, i, env, &formatted_input);
        else if (is_redirection(input[i]))
            i = handle_redirection_segment(input, i, &formatted_input);
        else if (input[i] == '|')
            i = handle_pipe_segment(input, i, &formatted_input);
        else if (isspace(input[i]) && quote_state == NO_QUOTE)
        {
            if (formatted_input[0] && !isspace(formatted_input[strlen(formatted_input) - 1]))
                formatted_input = append_char(formatted_input, ' ');
            i++;
        }
        else
            i = handle_word_segment(input, i, &formatted_input, env);

        if (i == -1) 
        {
            free(formatted_input);
            return NULL;
        }
    }

    return formatted_input;
}
char *replace_env_variables(const char *input)
{
    char *result = ft_strdup("");
    int i = 0;

    if (!result)
        return NULL;

    while (input[i])
    {
        if (input[i] == '$' && isalnum(input[i + 1]))
        {
            int start = ++i;
            while (isalnum(input[i]) || input[i] == '_')
                i++;
            char *var_name = strndup(input + start, i - start);
            char *var_value = getenv(var_name); // Simule env_find_var()
            free(var_name);
            char *temp = var_value ? append_char(result, *var_value) : append_char(result, ' ');
            free(result);
            result = temp;
        }
        else
            result = append_char(result, input[i++]);
    }
    return result;
}

/* ************************************************************************** */
/*                         FORMATTAGE & PARSING                               */
/* ************************************************************************** */

char *format_input(const char *input)
{
    char *formatted = ft_strdup("");
    int i = 0;
    t_quote_type quote_state = NO_QUOTE;

    while (input[i])
    {
        if (input[i] == '\'' || input[i] == '"')
        {
            quote_state = (quote_state == NO_QUOTE) ? (input[i] == '\'' ? SINGLE_QUOTE : DOUBLE_QUOTE) : NO_QUOTE;
            i++; // Ignore la quote
        }
        else if (input[i] == '\\' && quote_state != SINGLE_QUOTE)
        {
            if (input[i + 1] && (input[i + 1] == '$' || input[i + 1] == '"' || input[i + 1] == '\\'))
                i++; // Ignore le backslash
            formatted = append_char(formatted, input[i++]);
        }
        else if (input[i] == '$' && quote_state != SINGLE_QUOTE)
        {
            int start = ++i;
            while (isalnum(input[i]) || input[i] == '_')
                i++;
            char *var_name = strndup(input + start, i - start);
            char *var_value = getenv(var_name);
            free(var_name);
            if (var_value)
            {
                char *temp = ft_strdup(var_value);
                formatted = append_char(formatted, *temp);
                free(temp);
            }
        }
        else
            formatted = append_char(formatted, input[i++]);
    }

    return formatted;
}

void run_test(const char *input, const char *desired_output, t_env_var *env)
{
    char *result = format_input(input, env);
    printf("[INFO] Testing input: %s\n", input);
    printf("[INFO] Output: %s\n", result ? result : "(null)");
    printf("[INFO] Desired behavior: %s\n", desired_output);
    printf("\n");
    free(result);
}

void run_extended_tests(t_env_var *env)
{
    printf("========================================\n");
    printf("[INFO] Running extended tests...\n");

    // Liste des tests avec input et comportement attendu (desired output)
    run_test("echo \"text with \\\"escaped quotes\\\" inside\"", "echo text with \"escaped quotes\" inside", env);
    run_test("echo trailing\\\\ backslash", "echo trailing\\ backslash", env);
    run_test("echo test\\ value", "echo test value", env);
    run_test("echo \"A \\\"quoted\\\" word\"", "echo A \"quoted\" word", env);
    run_test("echo 'nested 'quotes''", "echo nested quotes", env);
    run_test("echo $USER$HOME", "echo sylvio/home/sylvio", env);
    run_test("cmd > > output", "cmd > > output", env);
    run_test("echo   $UNDEFINED", "echo ", env);
}

int main()
{
    t_env_var *env = NULL;

    // Initialisation de l'environnement avec des variables
    env_add_var(&env, env_new_var("USER", "sylvio"));
    env_add_var(&env, env_new_var("HOME", "/home/sylvio"));

    // Tests basiques
    printf("[INFO] Running basic tests...\n");
    run_test("Hello 'world' \"$USER\"", "Hello world sylvio", env);
    run_test("A > B >> C < D << EOF", "A > B >> C < D << EOF", env);
    run_test("$USER is here | grep 42", "sylvio is here | grep 42", env);
    run_test("Unclosed 'quote", "(null)", env);
    run_test("Escaped \\\"quote\\\" test", "(null)", env);
    run_test("Multiple \\$escaped $USER", "Multiple $escaped sylvio", env);

    // Tests étendus
    run_extended_tests(env);

    // Nettoyage de l'environnement
    while (env)
    {
        t_env_var *tmp = env->next;
        free(env->name);
        free(env->value);
        free(env);
        env = tmp;
    }

    printf("[INFO] Tests completed successfully!\n");
    return 0;
}

