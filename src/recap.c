
#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
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

extern int	g_child_pid; // PID du processus fils actif

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_APPEND,
	TOKEN_HEREDOC,
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
t_env_var	*copy_env(char **envp);
t_env_var	*env_new_var(char *name, char *value);
void		env_add_var(t_env_var **env, t_env_var *new_var);
t_env_var	*env_find_var(t_env_var *env, const char *name);
int			count_env_vars(t_env_var *env);
char		**env_to_array(t_env_var *env);
void		free_env(t_env_var *env);
void		free_env_array(char **env_array);
void		print_env(t_env_var *env);

/* Initialisation et signaux */
void		init_shell(t_shell *shell, char **envp);
void		setup_signals(void);
void		handle_signals(int signo);

/* Gestion des entrées utilisateur */
char		*read_user_input(t_shell *shell);
const char	*get_prompt_color(int last_exit_status);
char		*get_prompt(t_shell *shell);
void		process_input(t_shell *shell);

/* Tokenization */
t_token *tokenize_input(const char *input, t_shell *shell);
void		token_add(t_token **list, t_token *new_token);
t_token		*token_new(char *value, t_token_type type, t_quote_type quote);
void		token_free(t_token **list);

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
int	command_exists(const char *cmd, t_env_var *env);
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
int	command_exists(const char *cmd, t_env_var *env);
int	is_special_char(char c);
int	handle_special_char(const char *input, t_token **tokens, int i);
t_command	*organize_commands(t_token *tokens);
/*gestion exec*/
void		execute_commands(t_shell *shell, t_command *commands);
char	*resolve_command_path(char *command, t_env_var *env);
// Prototypes pour les commandes internes
void	set_env_var(t_env_var **env, char *name, char *value);
void	unset_env_var(t_env_var **env, char *name);
int		builtin_env(t_env_var *env);
int		builtin_export(char **args, t_env_var **env);
int	builtin_exit(char **args, t_shell *shell);
char	*get_env_value(t_env_var *env, const char *key);
int	change_directory(t_command *cmd, t_shell *shell);
void	update_env_vars(char *new_path, t_env_var **env);
void builtin_echo(char **args, t_shell *shell);
int	builtin_cd(t_command *cmd, t_shell *shell);
int		builtin_unset(char **args, t_env_var **env);
int		builtin_pwd(void);
int		execute_builtin(t_command *cmd, t_shell *shell);
int		is_builtin(const char *cmd);
int is_critical_builtin(const char *cmd);
int	is_valid_identifier(const char *str);
int 		handle_redirections(t_command *cmd);
int		validate_commands(t_command *commands);
/* Gestion des commandes */
//void		add_argument_token(const char *input, int start, int end, t_token **tokens, t_env_var *env);

/* Fonctions utilitaires */
char		*ft_strjoin_three(const char *s1, const char *s2, const char *s3);
char		*ft_strjoin_multiple(int count, ...);
int			array_len(char **array);
void		free_array(char **array);
char		*append_char(char *result, char c);
/*safe_utils*/
void	*safe_malloc(size_t size, t_shell *shell);
char	*safe_strdup(const char *s, t_shell *shell);
void	*safe_realloc(void *ptr, size_t old_size, size_t new_size, t_shell *shell);
void	print_error(const char *cmd, const char *msg);

/* ************************************************************************** */
#endif

#include "minishell.h"

int g_child_pid = 0; // Variable globale pour les signaux

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;

	// 1) Initialisation du shell (copie env, etc.)
	init_shell(&shell, envp);
	printf("[DEBUG] Shell initialized successfully.\n");

	// 2) Boucle principale
	while (shell.running)
	{
		// 3) Traitement d'un input utilisateur (et exécution)
		process_input(&shell);
	}

	// 4) Nettoyage des ressources
	printf("[DEBUG] Cleaning up resources.\n");
	free_env(shell.env);
	printf("[DEBUG] Shell exited successfully.\n");

	// 5) Retourner le code d'erreur / statut
	return (shell.exit_stat);
}
#include "minishell.h"

/* Copie les variables d'environnement en une liste chaînée */
t_env_var	*copy_env(char **envp)
{
	t_env_var	*env_list;
	t_env_var	*new_var;
	char		*name;
	char		*value;
	int			i;

	env_list = NULL;
	i = 0;
	while (envp && envp[i])
	{
		// [DEBUG] Ex: printf("[DEBUG] copy_env: processing envp[%d] = '%s'\n", i, envp[i]);

		// Trouve le '=' et copie name/value
		char *eq = ft_strchr(envp[i], '=');
		if (!eq)
		{
			// [OPTIONNEL] Gérer le cas d'une variable sans '='
			// continue ou break
		}
		name = ft_strndup(envp[i], eq - envp[i]);
		value = ft_strdup(eq + 1);
		if (!name || !value)
			return (NULL);

		new_var = env_new_var(name, value);
		env_add_var(&env_list, new_var);
		free(name);
		free(value);
		i++;
	}
	return (env_list);
}

/* Initialise le shell avec les variables d'environnement */
void	init_shell(t_shell *shell, char **envp)
{
	shell->env = copy_env(envp);
	if (!shell->env)
	{
		perror("Failed to initialize environment");
		exit(EXIT_FAILURE);
	}
	// Supprimer la valeur d'OLDPWD au démarrage
	//unset_env_var(&shell->env, "OLDPWD");
	shell->exit_stat = 0; // Initialisation du statut de sortie
	shell->running = 1;          // Le shell est actif
}
#include "minishell.h"

/* ************************************************************************** */
/*                      LECTURE DE L'ENTRÉE UTILISATEUR                       */
/* ************************************************************************** */

/* Lis l'entrée utilisateur via readline et retourne la commande */
char	*read_user_input(t_shell *shell)
{
	char	*prompt;
	char	*input;

	prompt = get_prompt(shell);
	if (!prompt)
	{
		perror("[ERROR] Failed to generate prompt");
		return (NULL);
	}

	input = readline(prompt);
	free(prompt);

	if (!input) // Gestion de Ctrl+D
	{
		printf("exit\n");
		shell->running = 0;
		return (NULL);
	}
	if (*input) // Si l'entrée n'est pas vide
		add_history(input);
	return (input);
}

/* ************************************************************************** */
/*                   TRAITEMENT ET AFFICHAGE DES TOKENS                       */
/* ************************************************************************** */

/* Processus de gestion d'une commande */



void	process_input(t_shell *shell)
{
	char		*input;
	t_token		*tokens;
	t_command	*commands;

	// 1) Installer les handlers de signaux au début
	setup_signals();

	// 2) Lire l'entrée utilisateur
	input = read_user_input(shell);
	if (!input || !shell->running)
		return ;

	// 3) Tokeniser l'entrée
	tokens = tokenize_input(input, shell);
	if (!tokens)
	{
		free(input);
		return ;
	}

	// 4) Valider la séquence de tokens
	if (!validate_token_sequence(tokens))
	{
		token_free(&tokens);
		free(input);
		return ;
	}

	// 5) Organiser les commandes (remplace parse_tokens)
	commands = organize_commands(tokens);
	token_free(&tokens);
	if (!commands)
	{
		free(input);
		return ;
	}

	// 6) Validation finale des commandes
	if (validate_commands(commands) < 0)
	{
		free_commands(commands);
		free(input);
		return ;
	}

	// 7) Exécuter les commandes
	execute_commands(shell, commands);

	// 8) Libérer
	free_commands(commands);
	free(input);
}

#include "minishell.h"

/* Retourne la couleur du prompt en fonction du statut de la dernière commande */
const char	*get_prompt_color(int last_exit_status)
{
	if (last_exit_status == 0)
		return ("\001" GREEN "\002");
	return ("\001" RED "\002");
}


/* Construit dynamiquement le prompt du shell */
char *get_prompt(t_shell *shell)
{
    const char *color;
    const char *base_prompt = "minishell> ";
    char *final_prompt;

    // Obtenir la couleur en fonction du statut
    color = get_prompt_color(shell->exit_stat);

    // Vérification des chaînes avant de les utiliser
    if (!color || !base_prompt)
    {
        fprintf(stderr, "[ERROR] get_prompt: Invalid color or base_prompt\n");
        exit(EXIT_FAILURE);
    }

    // Construction du prompt
    final_prompt = ft_strjoin_three(color, base_prompt, "\001" COLOR_RESET "\002");
    if (!final_prompt)
    {
        fprintf(stderr, "[ERROR] get_prompt: Failed to allocate memory for prompt\n");
        exit(EXIT_FAILURE);
    }

    return final_prompt;
}

#include "minishell.h"

char	*ft_strjoin_three(const char *s1, const char *s2, const char *s3, t_shell *shell)
{
	size_t	len;
	char	*result;

	if (!s1 || !s2 || !s3)
	{
		print_error("ft_strjoin_three", "NULL argument provided");
		return (NULL);
	}

	len = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3) + 1;
	result = safe_malloc(len, shell);
	if (!result)
		return (NULL);
	ft_strlcpy(result, s1, len);
	ft_strlcat(result, s2, len);
	ft_strlcat(result, s3, len);
	return (result);
}

char	*ft_strjoin_multiple(t_shell *shell, int count, ...)
{
	va_list	args;
	char	*result;
	char	*temp;
	size_t	total_len;
	int		i;

	total_len = 1;
	va_start(args, count);
	i = 0;
	while (i < count)
	{
		temp = va_arg(args, char *);
		if (temp)
			total_len += ft_strlen(temp);
		i++;
	}
	va_end(args);

	result = safe_malloc(total_len, shell);
	if (!result)
		return (NULL);
	*result = '\0';

	va_start(args, count);
	i = 0;
	while (i < count)
	{
		temp = va_arg(args, char *);
		if (!temp)
			temp = "";
		ft_strlcat(result, temp, total_len);
		i++;
	}
	va_end(args);
	return (result);
}



// Retourne la longueur d'un tableau 2D (null-terminated)
int	array_len(char **array)
{
	int	len;

	if (!array)
		return (0);
	len = 0;
	while (array && array[len])
		len++;
	return (len);
}

// Libère un tableau 2D (null-terminated)
void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void ft_lstclear(t_list **lst, void (*del)(void *))
{
    t_list *temp;

    // Vérification des arguments
    if (!lst || !del)
        return;

    while (*lst)
    {
        temp = (*lst)->next;
        del((*lst)->content);  // Libère le contenu de l'élément
        free(*lst);           // Libère l'élément lui-même
        *lst = temp;          // Passe à l'élément suivant
    }
}
#include "minishell.h"

void handle_signals(int signo)
{
    if (g_child_pid != 0) // S'il y a un processus en cours (comme `cat` ou `sleep`)
    {
        kill(g_child_pid, signo); // Envoyer le signal au fils
        return ;
    }
    if (signo == SIGINT) // Ctrl+C
    {
        printf("\n");
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
    else if (signo == SIGQUIT) // Ctrl+\ (à ignorer en mode interactif)
     {
        printf("\n"); // ✅ Assurer un retour à la ligne
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
}

void setup_signals(void)
{
    signal(SIGINT, handle_signals);
    signal(SIGQUIT, handle_signals); // On intercepte SIGQUIT
}

#include "minishell.h"

/* Crée un nouveau nœud de variable d'environnement */

t_env_var	*env_new_var(char *name, char *value, t_shell *shell)
{
	t_env_var	*new_var;

	new_var = safe_malloc(sizeof(t_env_var), shell);
	if (!new_var)
		return (NULL);
	new_var->name = safe_strdup(name, shell);
	if (!new_var->name)
	{
		free(new_var);
		return (NULL);
	}
	new_var->value = safe_strdup(value, shell);
	if (!new_var->value)
	{
		free(new_var->name);
		free(new_var);
		return (NULL);
	}
	new_var->next = NULL;
	return (new_var);
}

/* Ajoute une nouvelle variable à la liste */
void	env_add_var(t_env_var **env, t_env_var *new_var)
{
	if (!*env) // Si la liste est vide, ajoute directement
	{
		*env = new_var;
		return ;
	}

	t_env_var	*current = *env;
	while (current->next)
		current = current->next;
	current->next = new_var;
}

/* Recherche une variable d'environnement par nom */
t_env_var	*env_find_var(t_env_var *env, const char *name)
{
	while (env)
	{
		if (ft_strcmp(env->name, name) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

/* Libère toute la liste des variables d'environnement */
void	free_env(t_env_var *env)
{
	t_env_var	*tmp;

	while (env)
	{
		tmp = env->next;  // Stocke le prochain élément avant de free
		free(env->name);
		free(env->value);
		free(env);
		env = tmp;
	}
}

#include "minishell.h"

/* Compte le nombre de variables d'environnement */
int	count_env_vars(t_env_var *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

/* Transforme la liste chaînée en un tableau de type "key=value" */
char	**env_to_array(t_env_var *env, t_shell *shell)
{
	char	**env_array;
	int		count;
	int		i;

	count = count_env_vars(env);
	env_array = safe_malloc(sizeof(char *) * (count + 1), shell);
	if (!env_array)
		return (NULL);
	i = 0;
	while (env)
	{
		env_array[i] = ft_strjoin_three(env->name, "=", env->value, shell);
		if (!env_array[i])
		{
			free_env_array(env_array); // Libérer en cas d'erreur
			return (NULL);
		}
		env = env->next;
		i++;
	}
	env_array[i] = NULL;
	return (env_array);
}

/* Libère un tableau d'environnement */
void	free_env_array(char **env_array)
{
	int	i;

	if (!env_array)
		return ;
	i = 0;
	while (env_array[i])
	{
		free(env_array[i]);
		i++;
	}
	free(env_array);
}

void	print_env(t_env_var *env)
{
	while (env)
	{
		printf("%s=%s\n", env->name, env->value);
		env = env->next;
	}
}
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

#include "minishell.h"

/* ************************************************************************** */
/*                           FONCTION PRINCIPALE                              */
/* ************************************************************************** */


char	*replace_env_variables(const char *input, t_shell *shell)
{
	char	*result;
	int		i;

	result = safe_strdup("", shell); // 🔥 Utilisation sécurisée de safe_strdup
	if (!result)
		return (NULL); // On propage l'erreur proprement
	i = 0;
	while (input[i])
	{
		if (input[i] == '$' && (ft_isalnum(input[i + 1]) || input[i + 1] == '_' || input[i + 1] == '?'))
		{
			result = replace_variable(input, &i, result, shell);
			if (!result)
				return (NULL); // On arrête tout si erreur mémoire
		}
		else
		{
			result = append_char(result, input[i]);
			if (!result)
				return (NULL); // Même chose ici
			i++;
		}
	}
	return (result);
}

char	*replace_variable(const char *input, int *i, char *result, t_shell *shell)
{
	int		var_start;
	char	*var_name;
	char	*var_value;
	char	*temp;


	(*i)++;
	// 📌 **Cas spécial pour `$?`**
	if (input[*i] == '?')
	{
		var_value = ft_itoa(shell->exit_stat);
		(*i)++; // Avancer après `?`
	}
	else
	{
		var_start = *i;
		while (ft_isalnum(input[*i]) || input[*i] == '_')
			(*i)++;
		var_name = ft_substr(input, var_start, *i - var_start);
		if (!var_name)
			return (NULL);
		var_value = get_env_value(shell->env, var_name);
		free(var_name);
	}
	if (!var_value)
		var_value = safe_strdup("", shell); // Éviter NULL avec safe_strdup
	temp = ft_strjoin(result, var_value);
	free(result);
	free(var_value);
	return (temp);
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


char	*process_quotes_and_expansion(const char *input, t_shell *shell)
{
	char			*result;
	char			*sub;
	int				i;
	int				start;
	t_quote_type	quote_state;

	result = safe_strdup("", shell); // Meilleure initialisation avec safe_strdup
	if (!result)
		return (NULL);
	i = 0;
	quote_state = NO_QUOTE;
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '"')
		{
			quote_state = set_in_quotes(input[i], quote_state);
			start = ++i;
			while (input[i] && set_in_quotes(input[i], quote_state) == quote_state)
				i++;
			if (!input[i]) // Quote non fermée
			{
				free(result);
				return (NULL);
			}
			sub = ft_substr(input, start, i - start);
			if (!sub)
			{
				free(result);
				return (NULL);
			}
			if (quote_state == SINGLE_QUOTE)
				result = ft_strjoin_multiple(2, result, sub);
			else if (quote_state == DOUBLE_QUOTE)
			{
				sub = replace_env_variables(sub, shell);
				result = ft_strjoin_multiple(2, result, sub);
			}
			free(sub);
			quote_state = set_in_quotes(input[i++], quote_state);
		}
		else if (input[i] == '$' && (ft_isalnum(input[i + 1]) || input[i + 1] == '_' || input[i + 1] == '?'))
		{
			start = i;
			while (input[++i] && (ft_isalnum(input[i]) || input[i] == '_' || input[i] == '?'))
				;
			sub = ft_substr(input, start, i - start);
			sub = replace_env_variables(sub, shell);
			if (!sub)
			{
				free(result);
				return (NULL);
			}
			result = ft_strjoin_multiple(2, result, sub);
			free(sub);
		}
		else
		{
			char temp[2] = {input[i++], '\0'};
			result = ft_strjoin_multiple(2, result, temp);
		}
		if (!result)
			return (NULL);
	}
	return (result);
}
#include "minishell.h"

//actuellement parse_token est inutile et pas appelee
t_command	*parse_tokens(t_token *tokens)
{
	t_command	*commands = NULL;
	t_command	*current = NULL;

	while (tokens)
	{
		if (!current)
		{
			current = malloc(sizeof(t_command));
			if (!current)
				return (NULL);
			ft_bzero(current, sizeof(t_command));
			if (!commands)
				commands = current;
			else
			{
				t_command *temp = commands;
				while (temp->next)
					temp = temp->next;
				temp->next = current;
			}
		}
		if (tokens->type == TOKEN_WORD)
		{
			int argc = array_len(current->argv);
			current->argv = realloc(current->argv, sizeof(char *) * (argc + 2));
			current->argv[argc] = ft_strdup(tokens->value);
			current->argv[argc + 1] = NULL;
		}
		else if (tokens->type == TOKEN_REDIRECT_OUT && tokens->next)
		{
			current->output_file = ft_strdup(tokens->next->value);
			tokens = tokens->next;
		}
		else if (tokens->type == TOKEN_PIPE)
			current = NULL;
		tokens = tokens->next;
	}
	return (commands);
}
/*
char	*handle_heredoc(const char *delimiter)
{
	char	*line;
	char	*content = ft_strdup("");

	if (!content)
		return (NULL);
	while (1)
	{
		line = readline("minishell> ");
		if (!line || ft_strcmp(line, delimiter) == 0) // Délimiteur rencontré
			break;
		char *temp = content;
		content = ft_strjoin(content, line);
		content = ft_strjoin(content, "\n");
		free(temp);
		free(line);
	}
	free(line);
	return (content);
}
*/

void	free_commands(t_command *commands)
{
	t_command	*temp;

	while (commands)
	{
		// Sauvegarder le pointeur vers le nœud suivant
		temp = commands->next;

		// Libérer les arguments de la commande (tableau de chaînes)
		if (commands->argv)
			free_array(commands->argv);

		// Libérer les fichiers d'entrée et de sortie
		free(commands->input_file);
		free(commands->output_file);
		free(commands->heredoc_delimiter); // Libérer le délimiteur
		// Libérer la structure elle-même
		free(commands);

		// Passer au nœud suivant
		commands = temp;
	}
}


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
/* Ajoute un argument comme token après traitement des quotes et expansions fonction inutilisee depuis le 30 janv*/
/*
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
*/

#include "minishell.h"


int	validate_token_sequence(t_token *tokens)
{
	t_token	*current;
	int		is_separator;

	if (tokens && tokens->type == TOKEN_PIPE)
	{
		fprintf(stderr, "Error: command starts with a pipe\n");
		return (0);
	}

	current = tokens;
	is_separator = 0;
	while (current)
	{
		if (current->type == TOKEN_UNSUPPORTED)
		{
			fprintf(stderr, "Error: unsupported token detected\n");
			return (0);
		}
		if (current->type == TOKEN_PIPE
			|| current->type == TOKEN_REDIRECT_IN
			|| current->type == TOKEN_REDIRECT_OUT
			|| current->type == TOKEN_REDIRECT_APPEND
			|| current->type == TOKEN_HEREDOC)
		{
			if (is_separator)
			{
				fprintf(stderr, "Error: invalid sequence of tokens\n");
				return (0);
			}
			is_separator = 1;

			if (!current->next || current->next->type != TOKEN_WORD)
			{
				if (current->type == TOKEN_PIPE)
					fprintf(stderr, "Error: command ends with a pipe\n");
				else if (current->type == TOKEN_HEREDOC)
				{
					if (!current->next)
						fprintf(stderr, "Error: missing delimiter for heredoc\n");
					else
						return (1); // ✅ Accepte `<< eof`, donc pas une erreur !
				}
				else
					fprintf(stderr, "Error: missing file for redirection\n");
				return (0);
			}

			// ✅ Vérifie si un mot est collé directement après le délimiteur heredoc
			if (current->type == TOKEN_HEREDOC)
			{
				if (current->next->next && current->next->next->type == TOKEN_WORD)
				{
					fprintf(stderr, "Error: unexpected token after heredoc delimiter\n");
					return (0);
				}
			}
		}
		else
			is_separator = 0;
		current = current->next;
	}
	return (1);
}

int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	handle_special_char(const char *input, t_token **tokens, int i)
{
	if (input[i + 1] == input[i])
	{
		if (input[i] == '<')
			token_add(tokens, token_new(ft_substr(input, i, 2), TOKEN_REDIRECT_IN, NO_QUOTE));
		else
			token_add(tokens, token_new(ft_substr(input, i, 2), TOKEN_REDIRECT_OUT, NO_QUOTE));
		return (i + 2);
	}
	if (input[i] == '|')
		token_add(tokens, token_new(ft_substr(input, i, 1), TOKEN_PIPE, NO_QUOTE));
	else
		token_add(tokens, token_new(ft_substr(input, i, 1), TOKEN_REDIRECT_OUT, NO_QUOTE));
	return (i + 1);
}

/* Initialise une nouvelle structure t_command */
static t_command	*init_new_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);

	/* Tout mettre à 0 ou -1 */
	cmd->argv = NULL;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->append = 0;
	cmd->heredoc = 0; 
	cmd->heredoc_delimiter = NULL;
	cmd->pipe_in = -1;
	cmd->pipe_out = 0;
	cmd->next = NULL;
	return (cmd);
}

/*
** organize_commands:
** Convertit la liste de tokens en liste de commandes (chacune ayant argv[], redirections, etc.)
** - Crée une nouvelle commande (init_new_command()) si besoin (début ou après un pipe).
** - Stocke les arguments (TOKEN_WORD),
** - Stocke input_file/output_file selon les redirections,
** - Gère heredoc_delimiter si TOKEN_HEREDOC,
** - Gère pipe_in/pipe_out si TOKEN_PIPE.
*/
/*
/* Initialise une nouvelle structure t_command */
t_command	*init_new_command(t_shell *shell)
{
	t_command	*cmd;

	cmd = safe_malloc(sizeof(t_command), shell);
	if (!cmd)
		return (NULL);

	/* Tout initialiser proprement */
	cmd->argv = NULL;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->append = 0;
	cmd->heredoc = 0; 
	cmd->heredoc_delimiter = NULL;
	cmd->pipe_in = -1;
	cmd->pipe_out = 0;
	cmd->next = NULL;
	return (cmd);
}
t_command	*organize_commands(t_token *tokens, t_shell *shell)
{
	t_command	*commands;
	t_command	*current;
	t_command	*tmp;

	commands = NULL;
	current = NULL;

	while (tokens)
	{
		// Initialiser une nouvelle commande si aucune n'existe
		if (!current)
		{
			current = init_new_command(shell);
			if (!current)
			{
				print_error("organize_commands", "memory allocation failed");
				free_commands(commands);
				return (NULL);
			}
			if (!commands)
				commands = current;
			else
			{
				tmp = commands;
				while (tmp->next)
					tmp = tmp->next;
				tmp->next = current;
			}
		}

		// Gestion des tokens
		if (tokens->type == TOKEN_WORD)
		{
			char *arg = safe_strdup(tokens->value, shell);
			if (!arg)
			{
				print_error("organize_commands", "memory allocation failed for argument");
				free_commands(commands);
				return (NULL);
			}
			add_argument_to_command(current, arg, shell);
		}
		else if (tokens->type == TOKEN_PIPE)
		{
			current->pipe_out = 1;
			tokens = tokens->next;
			if (tokens)
			{
				current->next = init_new_command(shell);
				if (!current->next)
				{
					print_error("organize_commands", "memory allocation failed after pipe");
					free_commands(commands);
					return (NULL);
				}
				current->next->pipe_in = 1;
				current = current->next;
			}
			continue ;
		}
		else if (tokens->type == TOKEN_REDIRECT_IN && tokens->next && tokens->next->type == TOKEN_WORD)
		{
			if (!tokens->next->value)
			{
				print_error("organize_commands", "missing file for input redirection '<'");
				free_commands(commands);
				return (NULL);
			}
			current->input_file = safe_strdup(tokens->next->value, shell);
			tokens = tokens->next;
		}
		else if ((tokens->type == TOKEN_REDIRECT_OUT || tokens->type == TOKEN_REDIRECT_APPEND)
			&& tokens->next && tokens->next->type == TOKEN_WORD)
		{
			if (!tokens->next->value)
			{
				print_error("organize_commands", "missing file for output redirection '>'");
				free_commands(commands);
				return (NULL);
			}
			current->output_file = safe_strdup(tokens->next->value, shell);
			current->append = (tokens->type == TOKEN_REDIRECT_APPEND);
			tokens = tokens->next;
		}
		else if (tokens->type == TOKEN_HEREDOC && tokens->next && tokens->next->type == TOKEN_WORD)
		{
			if (!tokens->next->value)
			{
				print_error("organize_commands", "missing delimiter for heredoc");
				free_commands(commands);
				return (NULL);
			}
			current->heredoc_delimiter = safe_strdup(tokens->next->value, shell);
			current->heredoc = 1;
			tokens = tokens->next;
		}
		else if (tokens->type == TOKEN_UNSUPPORTED)
		{
			print_error("organize_commands", "unsupported token detected");
			free_commands(commands);
			return (NULL);
		}

		tokens = tokens->next;
	}

	if (!commands)
	{
		print_error("organize_commands", "no valid commands detected");
		return (NULL);
	}

	return (commands);
}
/*decoupage en plusieurs fonctions 

t_command	*organize_commands(t_token *tokens, t_shell *shell)
{
	t_command	*commands = NULL;
	t_command	*current = NULL;
	t_command	*tmp;

	while (tokens)
	{
		if (!current && !(current = init_new_command(shell)))
			return (free_commands(commands), NULL);
		if (!commands)
			commands = current;
		else
		{
			tmp = commands;
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = current;
		}
		if (!process_token(&tokens, current, shell))
			return (free_commands(commands), NULL);
		tokens = tokens->next;
	}
	return (commands);
}
int	process_token(t_token **tokens, t_command *current, t_shell *shell)
{
	if ((*tokens)->type == TOKEN_WORD)
		return (add_argument_to_command(current,
			safe_strdup((*tokens)->value, shell), shell));
	if ((*tokens)->type == TOKEN_PIPE)
		return (handle_pipe(tokens, current, shell));
	if ((*tokens)->type == TOKEN_REDIRECT_IN && (*tokens)->next)
		return (handle_redirection(tokens, current, shell, TOKEN_REDIRECT_IN));
	if (((*tokens)->type == TOKEN_REDIRECT_OUT || (*tokens)->type == TOKEN_REDIRECT_APPEND) && (*tokens)->next)
		return (handle_redirection(tokens, current, shell, (*tokens)->type));
	if ((*tokens)->type == TOKEN_HEREDOC && (*tokens)->next)
		return (handle_heredoc(tokens, current, shell));
	if ((*tokens)->type == TOKEN_UNSUPPORTED)
		return (print_error("organize_commands", "unsupported token detected"), 0);
	return (1);
}
int	handle_pipe(t_token **tokens, t_command *current, t_shell *shell)
{
	current->pipe_out = 1;
	(*tokens) = (*tokens)->next;
	if (*tokens && !(current->next = init_new_command(shell)))
		return (print_error("handle_pipe", "memory allocation failed"), 0);
	if (*tokens)
		current->next->pipe_in = 1;
	return (1);
}
int	handle_redirection(t_token **tokens, t_command *current, t_shell *shell, t_token_type type)
{
	if (!(*tokens)->next->value)
		return (print_error("organize_commands", "missing file for redirection"), 0);
	if (type == TOKEN_REDIRECT_IN)
		current->input_file = safe_strdup((*tokens)->next->value, shell);
	else
	{
		current->output_file = safe_strdup((*tokens)->next->value, shell);
		current->append = (type == TOKEN_REDIRECT_APPEND);
	}
	(*tokens) = (*tokens)->next;
	return (1);
}
int	handle_heredoc(t_token **tokens, t_command *current, t_shell *shell)
{
	if (!(*tokens)->next->value)
		return (print_error("organize_commands", "missing delimiter for heredoc"), 0);
	current->heredoc_delimiter = safe_strdup((*tokens)->next->value, shell);
	current->heredoc = 1;
	(*tokens) = (*tokens)->next;
	return (1);
}
*/ 

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
	printf("[DEBUG] token_add: Added token with value '%s'\n", new_token->value);
}

//Libère un token et une valeur en cas d'erreur 
static t_token	*free_token_and_value(char *value, t_token **tokens)
{
	if (value)
		free(value);
	if (tokens)
		token_free(tokens);
	return (NULL); // ✅ Retourne NULL pour éviter l'erreur de compilation
}

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
	while (input[i])
	{
		i = get_next_index(input, i);
		if (!input[i])
			break;

		/* Gestion des tokens `&&` ou `||` (non supportés) */
		if ((input[i] == '&' && input[i + 1] == '&') ||
			(input[i] == '|' && input[i + 1] == '|'))
		{
			raw_value = ft_substr(input, i, 2);
			if (!raw_value)
				return (free_token_and_value(NULL, &tokens));
			token_add(&tokens, token_new(raw_value, TOKEN_UNSUPPORTED, NO_QUOTE));
			i += 2;
			continue;
		}

		/* Gestion des pipes `|` */
		if (input[i] == '|')
		{
			raw_value = ft_substr(input, i, 1);
			if (!raw_value)
				return (free_token_and_value(NULL, &tokens));
			token_add(&tokens, token_new(raw_value, TOKEN_PIPE, NO_QUOTE));
			i++;
			continue;
		}

		/* Gestion des redirections `<`, `>`, `<<`, `>>` */
		if (input[i] == '<' || input[i] == '>')
		{
			if (input[i + 1] == input[i])
			{
				if (input[i] == '<')
					type = TOKEN_HEREDOC;
				else
					type = TOKEN_REDIRECT_APPEND;
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
				continue;
			}
			else
			{
				if (input[i] == '<')
					type = TOKEN_REDIRECT_IN;
				else
					type = TOKEN_REDIRECT_OUT;
				raw_value = ft_substr(input, i, 1);
				if (!raw_value)
					return (free_token_and_value(NULL, &tokens));
				token_add(&tokens, token_new(raw_value, type, NO_QUOTE));
				i++;
				continue;
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

#include "minishell.h"

void	debug_commands(t_command *commands)
{
	t_command	*cmd;
	int			i;

	printf("\n=== DEBUG COMMANDS ===\n");
	cmd = commands;
	while (cmd)
	{
		printf("Command:\n");
		printf("  Arguments:\n");
		for (i = 0; cmd->argv && cmd->argv[i]; i++)
			printf("    argv[%d]: '%s'\n", i, cmd->argv[i]);

		if (cmd->input_file)
			printf("  Input file: '%s'\n", cmd->input_file);
		else
			printf("  Input file: None\n");

		if (cmd->output_file)
			printf("  Output file: '%s'\n", cmd->output_file);
		else
			printf("  Output file: None\n");

		if (cmd->heredoc_delimiter)
			printf("  Heredoc Delimiter: '%s'\n", cmd->heredoc_delimiter);
		else
			printf("  Heredoc Delimiter: None\n");

		printf("  Append: %d\n", cmd->append);
		printf("  Pipe In: %d\n", cmd->pipe_in);
		printf("  Pipe Out: %d\n", cmd->pipe_out);
		printf("-----------------------\n");

		cmd = cmd->next;
	}
	printf("=== END OF COMMANDS ===\n\n");
}

/* Fonction pour afficher un debug des tokens générés */
void	debug_tokens(t_token *tokens)
{
	const char	*token_type_str[] = {
		"TOKEN_WORD",
		"TOKEN_PIPE",
		"TOKEN_REDIRECT_IN",
		"TOKEN_REDIRECT_OUT",
		"TOKEN_REDIRECT_APPEND",
		"TOKEN_HEREDOC"
	};

	t_token	*current = tokens;

	printf("[DEBUG] Tokens generated:\n");
	while (current)
	{
		printf("  [TOKEN] Type: %s, Value: '%s'\n",
			(current->type >= 0 && current->type <= TOKEN_HEREDOC) ? token_type_str[current->type] : "UNKNOWN",
			current->value);
		current = current->next;
	}
	printf("[DEBUG] End of Tokens.\n\n");
}

#include "minishell.h"

void cleanup_shell(t_shell *shell, t_command *commands, t_token *tokens, char *user_input)
{
	// 🔥 Tuer le processus enfant s'il existe encore
	if (g_child_pid > 0)
	{
		kill(g_child_pid, SIGKILL);
		g_child_pid = -1; // 🔥 On remet à -1 pour éviter les problèmes
	}

	// 🔥 Vérifie et libère l'environnement
	if (shell->env)
	{
		free_env(shell->env);
		shell->env = NULL;
	}

	// 🔥 Vérifie et libère les commandes
	if (commands)
	{
		free_commands(commands);
		commands = NULL;
	}

	// 🔥 Vérifie et libère les tokens
	if (tokens)
	{
		token_free(&tokens);
		tokens = NULL;
	}

	// 🔥 Vérifie et libère la ligne de commande utilisateur
	if (user_input)
	{
		free(user_input);
		user_input = NULL;
	}
}

void	*safe_malloc(size_t size, t_shell *shell)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
	{
		print_error("malloc", "memory allocation failed");
		shell->exit_stat = 1;  // ✅ On modifie ici car on détecte l'erreur
		return (NULL);
	}
	return (ptr);
}

char	*safe_strdup(const char *s, t_shell *shell)
{
	char	*copy;

	copy = ft_strdup(s);
	if (!copy)
	{
		print_error("strdup", "memory allocation failed");
		shell->exit_stat = 1;  // ✅ On modifie ici car on détecte l'erreur
		return (NULL);
	}
	return (copy);
}

void	*safe_realloc(void *ptr, size_t old_size, size_t new_size, t_shell *shell)
{
	void	*new_ptr;
	size_t	i;

	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	if (!ptr)
		return (safe_malloc(new_size, shell));
	new_ptr = safe_malloc(new_size, shell);
	if (!new_ptr)
		return (NULL);
	i = 0;
	while (i < old_size && i < new_size)
	{
		((char *)new_ptr)[i] = ((char *)ptr)[i];
		i++;
	}
	free(ptr);
	return (new_ptr);
}
void	print_error(const char *cmd, const char *msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (cmd)
	{
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

int	builtin_cd(t_command *cmd, t_shell *shell)
{
	char	*path;
	char	cwd[1024];
	char	*old_pwd;
	int		from_cd_dash;

	from_cd_dash = 0; // Flag pour indiquer si "cd -" ou "cd ." est utilisé

	// Obtenir le chemin actuel (PWD)
	if (!getcwd(cwd, sizeof(cwd)))
	{
		print_error("cd", "getcwd failed");
		shell->exit_stat = 1; // ✅ Mise à jour ici
		return (1);
	}

	// Si aucun argument, utiliser HOME
	if (!cmd->argv[1])
	{
		path = get_env_value(shell->env, "HOME");
		if (!path || !*path)
		{
			print_error("cd", "HOME not set");
			shell->exit_stat = 1; // ✅ Mise à jour ici
			return (1);
		}
		path = safe_strdup(path, shell);
		if (!path)  // ✅ Vérification immédiate !
			return (1);
	}
	// Si "cd -" utilisé, alterner entre OLDPWD et PWD
	else if (ft_strcmp(cmd->argv[1], "-") == 0)
	{
		path = get_env_value(shell->env, "OLDPWD");
		if (!path || !*path)
		{
			print_error("cd", "OLDPWD not set");
			shell->exit_stat = 1; // ✅ Mise à jour ici
			return (1);
		}
		path = safe_strdup(path, shell);
		if (!path)  // ✅ Vérification immédiate !
			return (1);
		from_cd_dash = 1;

		// Affiche OLDPWD immédiatement (spécifique à cd -)
		ft_putstr_fd(path, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
	}
	// Si "cd ." utilisé, marquer comme déjà affiché
	else if (ft_strcmp(cmd->argv[1], ".") == 0)
	{
		from_cd_dash = 1; // Indique qu'on ne doit rien afficher
		path = safe_strdup(cwd, shell); // Le chemin reste le même
		if (!path)  // ✅ Vérification immédiate !
			return (1);
	}
	// Sinon, utiliser le chemin donné
	else
	{
	path = safe_strdup(cmd->argv[1], shell);
	if (!path)  // ✅ Vérification immédiate !
		return (1);
	}
	// Vérifie si le chemin existe
	if (access(path, F_OK) != 0)
	{
		print_error("cd", "no such file or directory");
		free(path);
		shell->exit_stat = 1; // ✅ Mise à jour ici
		return (1);
	}

	// Changer de répertoire
	if (chdir(path) == -1)
	{
		print_error("cd", "chdir failed");
		free(path);
		shell->exit_stat = 1; // ✅ Mise à jour ici
		return (1);
	}

	// Mettre à jour OLDPWD et PWD
	old_pwd = get_env_value(shell->env, "PWD");
	if (old_pwd)
		set_env_var(&shell->env, "OLDPWD", old_pwd, shell);
	if (getcwd(cwd, sizeof(cwd)))
		set_env_var(&shell->env, "PWD", cwd, shell);
	else
	{
		print_error("cd", "getcwd failed");
		shell->exit_stat = 1; // ✅ Mise à jour ici
	}

	// Affiche le chemin si ce n’est pas déjà fait via "cd -" ou "cd ."
	if (!from_cd_dash)
	{
		ft_putstr_fd(cwd, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
	}

	free(path);
	shell->exit_stat = 0; // ✅ Mise à jour ici en cas de succès
	return (0);
}

static int	is_valid_n_option(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (0);
	i = 1;
	while (arg[++i])
		if (arg[i] != 'n')
			return (0);
	return (1);
}

void	builtin_echo(char **args, t_shell *shell)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;

	// Vérifie si -n est activé
	while (args[i] && is_valid_n_option(args[i]))
	{
		newline = 0;
		i++;
	}

	// Affichage des arguments avec gestion correcte des espaces
	while (args[i])
	{
		if (write(STDOUT_FILENO, args[i], ft_strlen(args[i])) == -1)
		{
			print_error("echo", "write error");
			shell->exit_stat = 1;
			return;
		}
		if (args[i + 1])
		{
			if (write(STDOUT_FILENO, " ", 1) == -1)
			{
				print_error("echo", "write error");
				shell->exit_stat = 1;
				return;
			}
		}
		i++;
	}

	// Empêcher le prompt de coller après `echo -n`
	if (!newline)
	{
		if (write(STDOUT_FILENO, " \033[0K", 5) == -1)
		{
			print_error("echo", "write error");
			shell->exit_stat = 1;
			return;
		}
	}

	if (newline && write(STDOUT_FILENO, "\n", 1) == -1)
	{
		print_error("echo", "write error");
		shell->exit_stat = 1;
		return;
	}

	shell->exit_stat = 0;
}

/*
void	builtin_echo(char **args, t_shell *shell)
{
	int	i;
	int	newline;
	int	first_word;

	i = 1;
	newline = 1;
	first_word = 1;

	// Vérification de l'option -n
	if (args[i] && ft_strcmp(args[i], "-n") == 0)
	{
		newline = 0;
		i++;
	}

	// Affichage des arguments (en ignorant les arguments vides)
	while (args[i])
	{
		if (ft_strlen(args[i]) > 0) // Ignorer les arguments vides
		{
			if (!first_word)
				ft_putstr_fd(" ", STDOUT_FILENO); // Ajouter un espace seulement après le premier mot
			ft_putstr_fd(args[i], STDOUT_FILENO);
			first_word = 0;
		}
		i++;
	}

	// Ajout d'un saut de ligne si nécessaire
	if (newline)
		ft_putstr_fd("\n", STDOUT_FILENO);

	shell->last_exit_status = 0;
}
*/

/* Met à jour les variables PWD et OLDPWD */
void	update_env_vars(char *new_path, t_env_var **env, t_shell *shell)
{
	char	*old_pwd;

	old_pwd = get_env_value(*env, "PWD");
	if (old_pwd)
		set_env_var(env, "OLDPWD", old_pwd, shell);
	set_env_var(env, "PWD", new_path, shell);
}

int	change_directory(t_command *cmd, t_shell *shell)
{
	char	cwd[1024];
	char	*path;

	if (!cmd->argv[1])
	{
		char *home = get_env_value(shell->env, "HOME");  // Stocke la valeur de HOME dans home
		if (!home)
		{
			print_error("cd", "HOME not set");
			return (1);
		}
		path = safe_strdup(home, shell);  // Maintenant, on duplique home sans l'écraser
	}
	else
		path = safe_strdup(cmd->argv[1], shell);

	if (!path)
		return (1);

	if (access(path, F_OK) != 0)
	{
		print_error("cd", path);
		print_error(NULL, "No such file or directory");
		free(path);
		return (1);
	}
	if (chdir(path) == -1)
	{
		print_error("cd", "chdir failed");
		free(path);
		return (1);
	}
	if (getcwd(cwd, sizeof(cwd)))
		update_env_vars(cwd, &shell->env, shell);
	else
		print_error("cd", "getcwd failed");

	free(path);
	return (0);
}

char	*get_env_value(t_env_var *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->name, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}
/*
int	handle_heredoc(t_command *cmd, t_shell *shell)
{
	int		pipe_fd[2];
	int		expansion;
	char	*line;

	if (!cmd->heredoc_delimiter)
		return (0);
	expansion = detect_delimiter_and_strip_quotes(&cmd->heredoc_delimiter);
	if (pipe(pipe_fd) < 0)
	{
		perror("pipe");
		return (-1);
	}
	while (1)
	{
		line = readline("heredoc> ");
		if (!line)
			break ;
		if (ft_strcmp(line, cmd->heredoc_delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (expansion)
		{
			char *expanded = replace_env_variables(line, shell);
			free(line);
			line = expanded;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	cmd->pipe_in = pipe_fd[0];
	return (0);
}
*/



int	handle_heredoc(t_command *cmd, t_shell *shell)
{
	int		pipe_fd[2];
	int		expansion;
	pid_t	pid;
	int		status;

	if (!cmd->heredoc_delimiter)
		return (0);

	expansion = detect_delimiter_and_strip_quotes(&cmd->heredoc_delimiter);
	if (pipe(pipe_fd) < 0)
	{
		perror("pipe");
		return (-1);
	}

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (-1);
	}

	if (pid == 0) // ➜ Processus fils (heredoc)
	{
		char	*line;

		signal(SIGINT, SIG_DFL); // ✅ Restaurer SIGINT pour interruption propre
		close(pipe_fd[0]); // ❌ Le fils n’a pas besoin du pipe de lecture

		while (1)
		{
			line = readline("heredoc> ");
			if (!line) // ✅ Si readline retourne NULL (CTRL+D)
			{
				fprintf(stderr, "bash: warning: here-document delimited by end-of-file (wanted `%s')\n", cmd->heredoc_delimiter);
				close(pipe_fd[1]); // ✅ Fermer proprement
				exit(0); // ✅ Fin propre
			}

			if (ft_strcmp(line, cmd->heredoc_delimiter) == 0)
			{
				free(line);
				break;
			}

			if (expansion)
			{
				char *expanded = replace_env_variables(line, shell);
				free(line);
				if (!expanded) // ✅ Éviter crash si expansion échoue
				{
					close(pipe_fd[1]);
					exit(1);
				}
				line = expanded;
			}

			if (write(pipe_fd[1], line, ft_strlen(line)) < 0 || write(pipe_fd[1], "\n", 1) < 0)
			{
				free(line);
				close(pipe_fd[1]);
				exit(1);
			}

			free(line);
		}

		close(pipe_fd[1]);
		exit(0);
	}

	// ➜ Processus parent (shell principal)
	close(pipe_fd[1]); // ❌ Le parent n’a pas besoin d’écrire dans le pipe
	waitpid(pid, &status, 0); // 🚀 Attend la fin du heredoc

	// ✅ Réactive les signaux après heredoc
	setup_signals();

	// ✅ Vérifie si `Ctrl+C` a été utilisé pour interrompre
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		close(pipe_fd[0]); // ❌ On ferme tout car Ctrl+C a été pressé
		shell->exit_stat = 130;
		return (-1);
	}

	// ✅ Si aucune commande ne suit `<< eof`, on ferme le pipe proprement sans crash
	if (!cmd->argv || !cmd->argv[0])
	{
		close(pipe_fd[0]);
		return (-1);
	}

	cmd->pipe_in = pipe_fd[0]; // ✅ Assigner la sortie du heredoc au pipe de lecture
	return (0);
}

void	add_argument_to_command(t_command *command, const char *argument, t_shell *shell)
{
	int	argc;
	size_t	old_size;
	size_t	new_size;
	char	**new_argv;

	if (!command || !argument)
		return ;
	argc = array_len(command->argv);
	old_size = sizeof(char *) * (argc + 1);
	new_size = sizeof(char *) * (argc + 2);
	new_argv = safe_realloc(command->argv, old_size, new_size, shell);
	if (!new_argv)
		return ; // ✅ Si realloc échoue, on ne modifie pas command->argv
	command->argv = new_argv;
	command->argv[argc] = safe_strdup(argument, shell);
	if (!command->argv[argc])
	{
		command->argv[argc] = NULL; // ✅ On évite un pointeur sauvage
		return ;
	}
	command->argv[argc + 1] = NULL;
}

int	detect_delimiter_and_strip_quotes(char **del_ptr)
{
	char	*del = *del_ptr;
	size_t	len = ft_strlen(del);

	if (len >= 2 && del[0] == '\'' && del[len - 1] == '\'')
	{
		char *tmp = ft_substr(del, 1, len - 2);
		free(del);
		*del_ptr = tmp;
		return (0);
	}
	else if (len >= 2 && del[0] == '"' && del[len - 1] == '"')
	{
		char *tmp = ft_substr(del, 1, len - 2);
		free(del);
		*del_ptr = tmp;
		return (0);
	}
	return (1);
}

int is_critical_builtin(const char *cmd)
{
    if (!cmd)
        return 0;
    // Mettez ici les builtins critiques
    if (ft_strcmp(cmd, "cd") == 0
     || ft_strcmp(cmd, "export") == 0
     || ft_strcmp(cmd, "unset") == 0
     || ft_strcmp(cmd, "exit") == 0)
        return 1;
    return 0;
}

void execute_commands(t_shell *shell, t_command *commands)
{
    t_command    *cmd;
    int            prev_pipe_fd;
    int            pipe_fd[2];

    cmd = commands;
    prev_pipe_fd = -1;
    while (cmd)
    {
        pipe_fd[0] = -1;
        pipe_fd[1] = -1;

        if (cmd->heredoc_delimiter)
        {
            if (handle_heredoc(cmd, shell) < 0)
                return ;
        }
        if (cmd->pipe_out)
        {
            if (pipe(pipe_fd) < 0)
                return ;
        }

        if (is_critical_builtin(cmd->argv[0]))
        {
            int saved_stdin = dup(STDIN_FILENO);
            int saved_stdout = dup(STDOUT_FILENO);

            if (prev_pipe_fd != -1)
            {
                dup2(prev_pipe_fd, STDIN_FILENO);
                close(prev_pipe_fd);
            }
            else if (cmd->pipe_in != -1)
            {
                dup2(cmd->pipe_in, STDIN_FILENO);
                close(cmd->pipe_in);
            }
            if (cmd->pipe_out)
            {
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[0]);
                close(pipe_fd[1]);
            }

            if (handle_redirections(cmd) < 0)
            {
                dup2(saved_stdin, STDIN_FILENO);
                dup2(saved_stdout, STDOUT_FILENO);
                close(saved_stdin);
                close(saved_stdout);
                return ;
            }

            shell->exit_stat = execute_builtin(cmd, shell);

            dup2(saved_stdin, STDIN_FILENO);
            dup2(saved_stdout, STDOUT_FILENO);
            close(saved_stdin);
            close(saved_stdout);

            if (cmd->pipe_out)
            {
                close(pipe_fd[1]);
                prev_pipe_fd = pipe_fd[0];
            }
            else
                prev_pipe_fd = -1;
        }
        else
        {
            pid_t    pid;
            int        status;

            pid = fork();
            if (pid == 0)
            {
                if (prev_pipe_fd != -1)
                {
                    dup2(prev_pipe_fd, STDIN_FILENO);
                    close(prev_pipe_fd);
                }
                else if (cmd->pipe_in != -1)
                {
                    dup2(cmd->pipe_in, STDIN_FILENO);
                    close(cmd->pipe_in);
                }
                if (cmd->pipe_out)
                {
                    dup2(pipe_fd[1], STDOUT_FILENO);
                    close(pipe_fd[0]);
                    close(pipe_fd[1]);
                }
                if (handle_redirections(cmd) < 0)
                    exit(1);
                if (is_builtin(cmd->argv[0]))
                    exit(execute_builtin(cmd, shell));
                else
                {
                    char *path = resolve_command_path(cmd->argv[0], shell->env, shell);
                    if (!path)
                    {
                        fprintf(stderr, "minishell: %s: command not found\n", cmd->argv[0]);
                        exit(127);
                    }
                    execve(path, cmd->argv, env_to_array(shell->env, shell));
                    perror("execve");
                    exit(127);
                }
            }
            else if (pid < 0) // Erreur lors du fork
            {
                perror("fork");
                return ;
            }
            else // Processus parent
            {
                g_child_pid = pid;
                waitpid(pid, &status, 0);

                if (WIFEXITED(status)) // Si le processus s'est terminé normalement
                {
                    shell->exit_stat = WEXITSTATUS(status);
                }
                else if (WIFSIGNALED(status)) // Si un signal l'a interrompu
                {
                    int sig = WTERMSIG(status);
                    shell->exit_stat = 128 + sig;

                 if (sig == SIGQUIT) 
                {
                    ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
                }
                else if (sig == SIGINT)
                {
                    ft_putstr_fd("\n", STDERR_FILENO);
                }

                }

                g_child_pid = 0;
            }

            if (prev_pipe_fd != -1)
                close(prev_pipe_fd);
            if (cmd->pipe_out)
            {
                close(pipe_fd[1]);
                prev_pipe_fd = pipe_fd[0];
            }
            else
                prev_pipe_fd = -1;
        }
        cmd = cmd->next;
    }
}

int	handle_redirections(t_command *cmd)
{
	int	fd;

	if (cmd->input_file)
	{
		fd = open(cmd->input_file, O_RDONLY);
		if (fd < 0)
		{
			perror("open input_file");
			return (-1);
		}
		if (dup2(fd, STDIN_FILENO) < 0)
		{
			perror("dup2 input_file");
			close(fd);
			return (-1);
		}
		close(fd);
	}
	if (cmd->output_file)
	{
		if (cmd->append)
			fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
		{
			perror("open output_file");
			return (-1);
		}
		if (dup2(fd, STDOUT_FILENO) < 0)
		{
			perror("dup2 output_file");
			close(fd);
			return (-1);
		}
		close(fd);
	}
	return (0);
}

char	*resolve_command_path(char *command, t_env_var *env, t_shell *shell)
{
	t_env_var	*path_var;
	char		**paths;
	char		*full_path;
	int			i;

	if (!command)
		return (NULL);
	if (command[0] == '/' || (command[0] == '.' && command[1] == '/'))
	{
		if (access(command, X_OK) == 0)
			return (ft_strdup(command));
		return (NULL);
	}
	path_var = env_find_var(env, "PATH");
	if (!path_var || !path_var->value)
		return (NULL);
	paths = ft_split(path_var->value, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin_three(paths[i], "/", command, shell);
		if (!full_path)
		{
			free_array(paths);
			return (NULL);
		}
		if (access(full_path, X_OK) == 0)
		{
			free_array(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_array(paths);
	return (NULL);
}
// Met à jour ou ajoute une variable d'environnement
void set_env_var(t_env_var **env, char *name, char *value, t_shell *shell)
{
    t_env_var *var;

    var = env_find_var(*env, name);
    if (var)
    {
        free(var->value);  // ✅ Libération de l'ancienne valeur
        var->value = safe_strdup(value, shell);
        if (!var->value)
            return ;
    }
    else
        env_add_var(env, env_new_var(name, value, shell));
}

// Supprime une variable d'environnement
void	unset_env_var(t_env_var **env, char *name)
{
	t_env_var	*current = *env;
	t_env_var	*prev = NULL;

	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(current->name);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}


// Affiche toutes les variables d'environnement
int	builtin_env(t_env_var *env)
{
	if (!env)
	{
		ft_putstr_fd("minishell: env: no environment variables\n", STDERR_FILENO);
		return (1);
	}
	while (env)
	{
		if (env->value)
		{
			ft_putstr_fd(env->name, STDOUT_FILENO);
			ft_putstr_fd("=", STDOUT_FILENO);
			ft_putstr_fd(env->value, STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		env = env->next;
	}
	return (0);
}

static int	is_valid_number(const char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);

	// 🔥 Ignorer les espaces avant le nombre
	while (*str && (*str == ' ' || *str == '\t'))
		str++;

	// 🔥 Gérer le signe (- ou +)
	if (*str == '-' || *str == '+')
		str++;

	// 🔥 Vérifier que la partie restante est bien un nombre
	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i])) // Doit être un chiffre
			return (0);
		i++;
	}

	// 🔥 Ignorer les espaces après le nombre
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;

	// 🔥 S'il y a encore un caractère, ce n'est pas un nombre valide
	if (str[i] != '\0')
		return (0);

	return (1);
}



static long long	ft_atoll(const char *str, int *error)
{
	int			sign;
	long long	result;
	int			i;

	sign = 1;
	result = 0;
	i = 0;
	*error = 0; // Par défaut, pas d'erreur

	while (str[i] == ' ' || str[i] == '\t') // Ignorer espaces
		i++;
	if (str[i] == '-' || str[i] == '+') // Gérer le signe
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (ft_isdigit(str[i]))
	{
		// 🔥 Vérifier dépassement **AVANT** d'ajouter le chiffre
		if (result > (LLONG_MAX / 10) ||
			(result == (LLONG_MAX / 10) && (str[i] - '0') > (LLONG_MAX % 10)))
		{
			*error = 1; // ⚠️ Dépassement détecté !
			return (sign == 1 ? LLONG_MAX : LLONG_MIN);
		}
		result = result * 10 + (str[i] - '0');
		i++;
	}

	// 🔥 Vérifier dépassement **après** application du signe
	if ((sign == 1 && result > LLONG_MAX) || (sign == -1 && -result < LLONG_MIN))
	{
		*error = 1;
		return (sign == 1 ? LLONG_MAX : LLONG_MIN);
	}

	return (result * sign);
}


int	builtin_exit(char **args, t_shell *shell)
{
	long long	exit_code;
	int			error;
	char		*trimmed;

	printf("exit\n"); // ✅ Affiche "exit" avant de quitter (comme Bash)

	if (args[1])
	{
		trimmed = ft_strtrim(args[1], " \t"); // ✅ Supprime les espaces autour
		if (!is_valid_number(trimmed)) // ✅ Vérifie si c'est un nombre valide
		{
			fprintf(stderr, "minishell: exit: %s: numeric argument required\n", args[1]); // ✅ Correction affichage erreur
			free(trimmed);
			shell->exit_stat = 255;
			exit(255);
		}

		exit_code = ft_atoll(trimmed, &error);
		free(trimmed);

		// ✅ Vérifie si dépassement de `long long`
		if (error)
		{
			fprintf(stderr, "minishell: exit: %s: numeric argument required\n", args[1]); // ✅ Correction affichage erreur
			shell->exit_stat = 255;
			exit(255);
		}

		if (args[2]) // ✅ Trop d'arguments (comme Bash)
		{
			fprintf(stderr, "minishell: exit: too many arguments\n");
			shell->exit_stat = 1;
			return (1); // ✅ Ne quitte pas immédiatement
		}

		exit_code %= 256; // ✅ Assure que l'exit est entre 0 et 255
		exit(exit_code);
	}

	exit(shell->exit_stat);
}


/*
int	builtin_export(char **args, t_env_var **env)
{
	char	*name;
	char	*value;
	char	*equal_pos;

	if (!args[1]) // Pas d'argument : afficher toutes les variables
		return (print_env(*env), 0);
	equal_pos = ft_strchr(args[1], '=');
	if (!equal_pos) // Pas de signe '=' dans l'argument
		return (ft_putstr_fd("minishell: export: invalid argument\n",
				STDERR_FILENO), 1);
	name = ft_strndup(args[1], equal_pos - args[1]);
	value = ft_strdup(equal_pos + 1);
	if (!name || !value)
		return (free(name), free(value), 1);
	set_env_var(env, name, value);
	free(name);
	free(value);
	return (0);
}
*/
/* Ajoute ou met à jour une variable d'environnement */
int	builtin_export(char **args, t_env_var **env, t_shell *shell)
{
	char	*name;
	char	*value;
	char	*equal_pos;

	if (!args[1]) // Pas d'argument : afficher toutes les variables
		return (print_env(*env), 0);

	equal_pos = ft_strchr(args[1], '=');
	if (equal_pos)
		name = ft_strndup(args[1], equal_pos - args[1]); // Récupère le nom avant '='
	else
		name = safe_strdup(args[1], shell); // Aucun '=' => variable seule

	if (!name)
		return (1);

	if (!is_valid_identifier(name)) // Vérification de l'identifiant
	{
		print_error("export", "not a valid identifier");
		free(name);
		return (1);
	}

	if (equal_pos) // Ajout ou mise à jour de la variable
	{
		value = safe_strdup(equal_pos + 1, shell);
		if (!value)
		{
			free(name);
			return (1);
		}
		set_env_var(env, name, value, shell);
		free(value);
	}
	free(name);
	return (0); // Succès
}


// Supprime une variable d'environnement
int	builtin_unset(char **args, t_env_var **env)
{
	if (!args[1])
	{
		ft_putstr_fd("minishell: unset: missing argument\n", STDERR_FILENO);
		return (1);
	}
	unset_env_var(env, args[1]);
	return (0);
}

int	builtin_pwd(void)
{
	char	buffer[1024];

	if (getcwd(buffer, sizeof(buffer)))
	{
		ft_putstr_fd(buffer, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		return (0);
	}
	perror("pwd");
	return (1);
}



int	is_builtin(const char *cmd)
{
	if (ft_strcmp(cmd, "echo") == 0 ||
		ft_strcmp(cmd, "cd") == 0 ||
		ft_strcmp(cmd, "pwd") == 0 ||
		ft_strcmp(cmd, "export") == 0 ||
		ft_strcmp(cmd, "unset") == 0 ||
		ft_strcmp(cmd, "env") == 0 ||
		ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

int	execute_builtin(t_command *cmd, t_shell *shell)
{
	int	ret;

	// Gérer les redirections avant d'exécuter un builtin
	if (handle_redirections(cmd) < 0)
	{
		shell->exit_stat = 1;
		return (1);
	}

	// Exécuter le builtin approprié
	if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		ret = builtin_pwd();
	else if (ft_strcmp(cmd->argv[0], "export") == 0)
		ret = builtin_export(cmd->argv, &shell->env, shell);
	else if (ft_strcmp(cmd->argv[0], "unset") == 0)
		ret = builtin_unset(cmd->argv, &shell->env);
	else if (ft_strcmp(cmd->argv[0], "cd") == 0)
		ret = builtin_cd(cmd, shell);
	else if (ft_strcmp(cmd->argv[0], "env") == 0)
		ret = builtin_env(shell->env);
	else if (ft_strcmp(cmd->argv[0], "exit") == 0)
		ret = builtin_exit(cmd->argv, shell);
	else if (ft_strcmp(cmd->argv[0], "echo") == 0)
	{
		builtin_echo(cmd->argv, shell);
		ret = 0;
	}
	else
		ret = 1; // Si aucune commande builtin ne correspond

	// Mettre à jour le statut de sortie
	shell->exit_stat = ret;
	return (ret);
}

int	validate_commands(t_command *commands)
{
	t_command	*cmd;

	cmd = commands;
	while (cmd)
	{
		if (!cmd->argv || !cmd->argv[0])
		{
			// ✅ Si c'est un heredoc sans commande, on l'accepte !
			if (cmd->heredoc)
				return (0);
			print_error("syntax error", "unexpected empty command");
			return (-1);
		}
		if (cmd->pipe_out && (!cmd->next || !cmd->next->argv || !cmd->next->argv[0]))
		{
			print_error("syntax error", "near unexpected token '|'");
			return (-1);
		}
		if (cmd->input_file && access(cmd->input_file, F_OK) != 0)
		{
			print_error(cmd->input_file, "No such file or directory");
			return (-1);
		}
		cmd = cmd->next;
	}
	return (0);
}

int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0); // Doit commencer par une lettre ou '_'
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0); // Doit contenir uniquement lettres, chiffres ou '_'
		i++;
	}
	return (1);
}

