#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Structures
typedef struct s_env_var {
    char *name;
    char *value;
    struct s_env_var *next;
} t_env_var;

// Quotes
typedef enum e_quote_type
{
    QUOTE_NONE,
    QUOTE_SINGLE,
    QUOTE_DOUBLE
} t_quote_type;

#define SINGLE_QUOTE 1
#define DOUBLE_QUOTE 2

// Prototypes des fonctions principales
char *format_input(const char *input, t_env_var *env);

// Prototypes des modules
int handle_quotes_segment(const char *input, int i, t_env_var *env, char **formatted_input, int *quote_state);
int handle_expansion_segment(const char *input, int i, t_env_var *env, char **formatted_input);
int handle_redirection_segment(const char *input, int i, char **formatted_input);
int handle_pipe_segment(const char *input, int i, char **formatted_input);
int handle_word_segment(const char *input, int i, char **formatted_input, t_env_var *env);
int handle_backslash_segment(const char *input, int i, char **formatted_input, int quote_state);
char *replace_env_variables(const char *input, t_env_var *env);
// Prototypes des utilitaires
t_env_var *env_new_var(const char *name, const char *value);
void env_add_var(t_env_var **env, t_env_var *new_var);
t_env_var *env_find_var(t_env_var *env, const char *name);
char *ft_strdup(const char *s);
char *ft_strjoin(const char *s1, const char *s2);
char *ft_substr(const char *s, unsigned int start, size_t len);
char *append_char(char *result, char c);
int is_redirection(char c);

#endif

