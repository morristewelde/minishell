/*
#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h" 

# define COLOR_RESET "\033[0m"
# define GREEN       "\033[32m"
# define RED         "\033[31m"
# define LRED        "\033[38;5;203m"
# define YELLOW      "\033[33m"
# define ORANGE      "\033[38;5;214m"



extern int	g_child_pid; // PID du processus fils actif

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_APPEND,
	TOKEN_HEREDOC
} 	t_token_type;

typedef enum e_quote_type
{
	NO_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE
} 	t_quote_type;

typedef struct s_token
{
	char			*value;         // Valeur du token
	t_token_type	type;           // Type de token
	t_quote_type	quote;          // Type de guillemets, si applicable
	struct s_token	*next;          // Token suivant
} 	t_token;

typedef struct s_command
{
	char			**argv;         // Arguments de la commande
	char			*input_file;    // Fichier d'entrée
	char			*output_file;   // Fichier de sortie
	int			append;         // Mode append pour la redirection de sortie
	int			heredoc;        // Indique si c'est un heredoc
	struct s_command	*next;         // Commande suivante
} 	t_command;

typedef struct s_env_var
{
	char			*name;          // Nom de la variable d'environnement
	char			*value;         // Valeur de la variable
	struct s_env_var	*next;         // Variable suivante
} 	t_env_var;

typedef struct s_shell
{
	t_env_var		*env;           // Liste chaînée des variables d'environnement
	int			last_exit_status; // Statut de la dernière commande
	int			running;        // Booléen indiquant si le shell est actif
} 	t_shell;

// Prototypes : gestion des variables d’environnement 
t_env_var	*copy_env(char **envp);
t_env_var	*env_new_var(char *name, char *value);
void		env_add_var(t_env_var **env, t_env_var *new_var);
t_env_var	*env_find_var(t_env_var *env, const char *name);
int			count_env_vars(t_env_var *env);
char		**env_to_array(t_env_var *env);
void		free_env(t_env_var *env);
void		free_env_array(char **env_array);
void		print_env(t_env_var *env);

// Prototypes : initialisation du shell //
void		init_shell(t_shell *shell, char **envp);
void		setup_signals(void);
void		handle_signals(int signo);

// Prototypes : gestion des entrées utilisateur //
char		*read_user_input(t_shell *shell);
const char	*get_prompt_color(int last_exit_status);
char		*get_prompt(t_shell *shell);
void		process_input(t_shell *shell);
char		*ft_strjoin_three(const char *s1, const char *s2, const char *s3);
char *ft_strjoin_multiple(int count, ...);
// Token.c
void		token_add(t_token **list, t_token *new_token);
t_token	*token_new(char *value, t_token_type type, t_quote_type quote);

void		token_free(t_token **list);
t_token	*tokenize_input(const char *input, t_env_var *env);

// Parser.c
t_command	*parse_tokens(t_token *tokens);
void		free_commands(t_command *commands);



// Trouve l'index de fermeture des quotes (simple ou double)
int		get_end_of_quotes(const char *str, int start);
t_quote_type set_in_quotes(char c, t_quote_type current_quote);
// Trouve la fin d'un argument (prend en compte quotes et caractères spéciaux)
int		get_end_of_argument(const char *str, int start);
int	check_quotes(const char *input);
// Avance jusqu'au prochain argument ou token (ignore les espaces)
int		get_next_index(const char *str, int start);
char	*append_char(char *result, char c);
char	*replace_variable(const char *input, int *i, char *result, t_env_var *env);
char	*process_quotes_and_expansion(const char *input, t_env_var *env);
void	add_argument_token(const char *input, int start, int end, t_token **tokens, t_env_var *env);
int	validate_token_sequence(t_token *tokens);
int	is_special_char(char c);
int	handle_special_char(const char *input, t_token **tokens, int i);
int			array_len(char **array);
void		free_array(char **array);
#endif
*/
#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <signal.h>
# include <fcntl.h>
# include <limits.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <termios.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h" 

/* ************************************************************************** */
/*                             DEFINITIONS DE COULEURS                        */
/* ************************************************************************** */
# define COLOR_RESET "\033[0m"
# define GREEN       "\033[32m"
# define RED         "\033[31m"
# define LRED        "\033[38;5;203m"
# define YELLOW      "\033[33m"
# define ORANGE      "\033[38;5;214m"

/* ************************************************************************** */
/*                              TYPES ET STRUCTURES                           */
/* ************************************************************************** */

extern volatile sig_atomic_t g_child_pid; // PID du processus fils actif

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_APPEND,
	TOKEN_HEREDOC,
	TOKEN_SEMICOLON,
	TOKEN_UNSUPPORTED // Ajout pour gérer les tokens comme && et ||
} 	t_token_type;

typedef enum e_quote_type
{
	NO_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE
} 	t_quote_type;

typedef struct s_token
{
	char			*value;         // Valeur du token
	t_token_type	type;           // Type de token
	t_quote_type	quote;          // Type de guillemets, si applicable
	struct s_token	*next;          // Token suivant
} 	t_token;

typedef struct s_command
{
	char			**argv;
	char			*input_file;
	char			*output_file;
	int				append;
	int				heredoc;
	char    *heredoc_delimiter; // Nouveau champ
	int				pipe_in;  // Indique si la commande reçoit son entrée d'un pipe
	int				pipe_out; // Indique si la commande envoie sa sortie à un pipe
	struct s_command *next;
} 	t_command;

typedef struct s_env_var
{
	char			*name;          // Nom de la variable d'environnement
	char			*value;         // Valeur de la variable
	struct s_env_var	*next;         // Variable suivante
} 	t_env_var;

typedef struct s_shell
{
	t_env_var		*env;           // Liste chaînée des variables d'environnement
	int			exit_stat; // Statut de la dernière commande
	int			running;        // Booléen indiquant si le shell est actif
} 	t_shell;

/* ************************************************************************** */
/*                          PROTOTYPES DES FONCTIONS                          */
/* ************************************************************************** */

/* Gestion des variables d'environnement */
t_env_var       *copy_env(char **envp, t_shell *shell);
 t_env_var       *env_new_var(char *name, char *value, t_shell *shell);
void		env_add_var(t_env_var **env, t_env_var *new_var);
t_env_var	*env_find_var(t_env_var *env, const char *name);
int			count_env_vars(t_env_var *env);

void		free_env(t_env_var *env);
char    	**env_to_array(t_env_var *env, t_shell *shell);

void		free_env_array(char **env_array);
void		print_env(t_env_var *env);

/* Initialisation et signaux */
void		init_shell(t_shell *shell, char **envp);
void		setup_signals(void);
void		disable_echoctl(void);
void		handle_signals(int signo);

/* Gestion des entrées utilisateur */
char		*read_user_input(t_shell *shell);
const char	*get_prompt_color(int last_exit_status);
char		*get_prompt(t_shell *shell);
void		process_input(t_shell *shell);

/* Tokenization */
t_token *tokenize_input(const char *input, t_shell *shell);

// Gestion des opérateurs spéciaux (&&, ||, |)
int	tokenize_operators(t_token **tokens, const char *input, int *i);

// Gestion des redirections (<, >, <<, >>)
int	tokenize_redirections(t_token **tokens, const char *input, int *i);

// Gestion des arguments (mots, expansions, quotes)
int	tokenize_arguments(t_token **tokens, const char *input,
			int *i, t_shell *shell);

// Gestion du délimiteur dans les heredocs
int	tokenize_heredoc(t_token **tokens, const char *input, int *i);
void		token_add(t_token **list, t_token *new_token);
t_token		*token_new(char *value, t_token_type type, t_quote_type quote);
void		token_free(t_token **list);
t_token *get_last_token(t_token *tokens);
/* Parsing */
t_command	*parse_tokens(t_token *tokens);
void		free_commands(t_command *commands);

//char		*handle_heredoc(const char *delimiter);
/*parsing heredoc*/
int detect_delimiter_and_strip_quotes(char **del_ptr);
 int     handle_heredoc(t_command *cmd, t_shell *shell);

//char *expand_variables_only(const char *input, t_shell *shell);
//void add_argument_token(const char *input, int start, int end, t_token **tokens, t_shell *shell);
/* Validation et gestion des tokens */
int			validate_token_sequence(t_token *tokens);
int			is_special_char(char c);
int			handle_special_char(const char *input, t_token **tokens, int i);
//int	command_exists(const char *cmd, t_env_var *env);
//DEBUG
void	debug_commands(t_command *commands);
void	debug_tokens(t_token *tokens);
/* Gestion des quotes et expansions */
int			check_quotes(const char *input);
t_quote_type	set_in_quotes(char c, t_quote_type current_quote);
int			get_end_of_quotes(const char *str, int start);
int			get_end_of_argument(const char *str, int start);
int			get_next_index(const char *str, int start);
char *replace_env_variables(const char *input, t_shell *shell);
char *replace_variable(const char *input, int *i, char *result, t_shell *shell);
char *process_quotes_and_expansion(const char *input, t_shell *shell);
void	add_argument_to_command(t_command *command, const char *argument, t_shell *shell);
t_command	*init_new_command(t_shell *shell);
int	handle_single_token(t_token **tokens, t_command **cur, t_command **cmds, t_shell *shell);
int	handle_pipe_token(t_token **tokens, t_command **cur, t_command **cmds, t_shell *shell);
int	handle_word_token(t_command *cur, t_token *tok, t_shell *shell, t_command *cmds);
t_command	*organize_commands(t_token *tokens, t_shell *shell);
/*gestion exec*/
void		execute_commands(t_shell *shell, t_command *commands);
char	*resolve_command_path(char *command, t_env_var *env, t_shell *shell);
// Prototypes pour les commandes internes
void    set_env_var(t_env_var **env, char *name, char *value, t_shell *shell);
void	unset_env_var(t_env_var **env, char *name);
int		builtin_env(t_env_var *env);
int     builtin_export(char **args, t_env_var **env, t_shell *shell);
int	builtin_exit(char **args, t_shell *shell);
char	*get_env_value(t_env_var *env, const char *key);
int change_directory(const char *path, t_shell *shell);
char    *build_logical_path(const char *old_pwd, const char *arg);
void    update_env_vars(t_env_var **env, const char *new_logical_path, t_shell *shell);
void builtin_echo(char **args, t_shell *shell);
int	builtin_cd(t_command *cmd, t_shell *shell);
int		builtin_unset(char **args, t_env_var **env);
//int		builtin_pwd(void);
int		builtin_pwd(t_shell *shell);
int		execute_builtin(t_command *cmd, t_shell *shell);
int		is_builtin(const char *cmd);
int is_critical_builtin(const char *cmd);
int	is_valid_identifier(const char *str);
int 		handle_redirections(t_command *cmd);
int		validate_commands(t_command *commands);
/* Gestion des commandes */
//void		add_argument_token(const char *input, int start, int end, t_token **tokens, t_env_var *env);

/* Fonctions utilitaires */
char		*ft_strjoin_three(const char *s1, const char *s2, const char *s3, t_shell *shell);
char		*ft_strjoin_multiple(t_shell *shell, int count, ...);
int			array_len(char **array);
void		free_array(char **array);
char		*append_char(char *result, char c);
/*safe_utils*/
void	*safe_malloc(size_t size, t_shell *shell);
char	*safe_strdup(const char *s, t_shell *shell);
void	*safe_realloc(void *ptr, size_t old_size, size_t new_size, t_shell *shell);
void	print_error(const char *cmd, const char *msg);
void cleanup_shell(t_shell *shell, t_command *commands, t_token *tokens, char *user_input);
/* ************************************************************************** */
#endif

