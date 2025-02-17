#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
/*
// Simulation de l'environnement
typedef struct s_env_var {
    char *name;
    char *value;
    struct s_env_var *next;
} t_env_var;

t_env_var *env_new_var(const char *name, const char *value) {
    t_env_var *new_var = malloc(sizeof(t_env_var));
    if (!new_var) return NULL;
    new_var->name = strdup(name);
    new_var->value = strdup(value);
    new_var->next = NULL;
    return new_var;
}

void env_add_var(t_env_var **env, t_env_var *new_var) {
    new_var->next = *env;
    *env = new_var;
}

t_env_var *env_find_var(t_env_var *env, const char *name) {
    while (env) {
        if (strcmp(env->name, name) == 0)
            return env;
        env = env->next;
    }
    return NULL;
}

// Fonction utilitaire
char *ft_strjoin(const char *s1, const char *s2) {
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    if (!result) return NULL;
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

char *append_char(char *result, char c) {
    size_t len = strlen(result);
    char *new_result = malloc(len + 2);
    if (!new_result) return NULL;
    strcpy(new_result, result);
    new_result[len] = c;
    new_result[len + 1] = '\0';
    free(result);
    return new_result;
}

// Fonction principale qui remplace les variables et gère les quotes/backslashes
char *expand_and_clean(const char *input, t_env_var *env) {
    char *result = strdup("");
    int i = 0, quote_state = 0;

    if (!result) return NULL;

    while (input[i]) {
        if (input[i] == '"' && quote_state == 0) 
            quote_state = 1; 
        else if (input[i] == '"' && quote_state == 1) 
            quote_state = 0; 
        else if (input[i] == '\\' && quote_state == 1 && (input[i + 1] == '"' || input[i + 1] == '\\')) 
            i++;
        else if (input[i] == '$' && quote_state != 1) {
            int start = ++i;
            while (isalnum(input[i]) || input[i] == '_') 
                i++;
            char *var_name = strndup(&input[start], i - start);
            t_env_var *env_var = env_find_var(env, var_name);
            char *var_value = env_var ? strdup(env_var->value) : strdup("");
            free(var_name);
            char *temp = ft_strjoin(result, var_value);
            free(result);
            free(var_value);
            result = temp;
            continue;
        } else {
            result = append_char(result, input[i]);
        }
        i++;
    }
    return result;
}

// Fonction de test
void run_test(const char *input, const char *expected, t_env_var *env) {
    char *result = expand_and_clean(input, env);
    printf("Expected: %s | Got: %s\n", expected, result);
    free(result);
}

// Main avec uniquement les tests problématiques
int main() {
    t_env_var *env = NULL;
    env_add_var(&env, env_new_var("USER", "sylvio"));
    env_add_var(&env, env_new_var("HOME", "/home/sylvio"));

    printf("========================================\n");
    printf("[INFO] Running failing tests only...\n");

    run_test("echo \"text with \\\"escaped quotes\\\" inside\"", "echo text with \"escaped quotes\" inside", env);
    run_test("echo trailing\\\\ backslash", "echo trailing\\ backslash", env);
    run_test("echo \"A \\\"quoted\\\" word\"", "echo A \"quoted\" word", env);
    run_test("Escaped \\\"quote\\\" test", "(null)", env);

    printf("[INFO] Debugging session completed!\n");

    return 0;
}
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum e_quote_type
{
    NO_QUOTE,
    SINGLE_QUOTE,
    DOUBLE_QUOTE
} t_quote_type;

/* ************************************************************************** */
/*                         UTILITAIRES                                        */
/* ************************************************************************** */

char *ft_strdup(const char *s)
{
    char *dup = malloc(strlen(s) + 1);
    if (!dup)
        return NULL;
    strcpy(dup, s);
    return dup;
}

char *append_char(char *str, char c)
{
    size_t len = str ? strlen(str) : 0;
    char *new_str = malloc(len + 2);
    if (!new_str)
        return NULL;
    if (str)
        strcpy(new_str, str);
    new_str[len] = c;
    new_str[len + 1] = '\0';
    free(str);
    return new_str;
}

char *replace_env_variables(const char *input **env)
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

/* ************************************************************************** */
/*                         TESTS                                              */
/* ************************************************************************** */

void run_test(const char *input, const char *expected)
{
    char *result = format_input(input);
    printf("Expected: %s | Got: %s\n", expected, result);
    free(result);
}

int main()
{
    printf("==== Testing Backslashes & Quotes ====\n\n");

    run_test("echo \"text with \\\"escaped quotes\\\" inside\"", "echo text with \"escaped quotes\" inside");
    run_test("echo trailing\\ backslash", "echo trailing backslash");
    run_test("echo \"A \\\"quoted\\\" word\"", "echo A \"quoted\" word");
    run_test("Escaped \\\"quote\\\" test", "(null)");

    return 0;
}

