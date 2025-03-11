#include "minishell.h"


/*
** Construit un chemin logique à partir de l'ancien PWD et de l'argument.
** - Si 'arg' est absolu (commence par '/'), on le prend tel quel.
** - Sinon, on concatène old_pwd + "/" + arg.
** Retourne la nouvelle chaîne (à free plus tard si besoin).
*/

char    *concatenate_path(const char *old_pwd, const char *arg, int len_old, int len_arg)
{
    char    *new_path;
    int     i;
    int     j;

    new_path = malloc(sizeof(char) * (len_old + 1 + len_arg + 1));
    if (!new_path)
        return (NULL);

    i = 0;
    while (old_pwd && old_pwd[i])
    {
        new_path[i] = old_pwd[i];
        i++;
    }
    if (i > 0 && new_path[i - 1] != '/')
    {
        new_path[i] = '/';
        i++;
    }
    j = 0;
    while (arg[j])
        new_path[i++] = arg[j++];
    new_path[i] = '\0';
    return (new_path);
}

char    *build_logical_path(const char *old_pwd, const char *arg)
{
    int     len_old;
    int     len_arg;

    if (!arg)
        return (NULL);
    if (arg[0] == '/')
        return (ft_strdup(arg)); // Adaptez avec votre fonction de duplication

    len_old = ft_strlen(old_pwd);
    len_arg = ft_strlen(arg);
    return (concatenate_path(old_pwd, arg, len_old, len_arg));
}

/*
** Met à jour OLDPWD avec l'ancien PWD, puis PWD avec new_logical_path.
*/
void    update_env_vars(t_env_var **env, const char *new_logical_path, t_shell *shell)
{
    char    *old_pwd;

    old_pwd = get_env_value(*env, "PWD");
    if (old_pwd)
        set_env_var(env, "OLDPWD", old_pwd, shell);
    if (new_logical_path && *new_logical_path)
        set_env_var(env, "PWD", (char *)new_logical_path, shell);
}

/*
** change_directory:
** 1. Construit le chemin logique.
** 2. Appelle chdir().
** 3. Si getcwd() réussit -> PWD = getcwd().
**    Sinon -> on garde le chemin logique construit.
*/
int handle_cd_errors(const char *path, t_shell *shell, char *new_logical_path)
{
    if (!new_logical_path)
    {
        print_error("cd", "malloc failed");
        shell->exit_stat = 1;
        return (1);
    }
    if (access(path, F_OK) != 0)
    {
        print_error("cd", path);
        print_error(NULL, "No such file or directory");
        free(new_logical_path);
        shell->exit_stat = 1;
        return (1);
    }
    if (chdir(path) == -1)
    {
        print_error("cd", "chdir failed");
        free(new_logical_path);
        shell->exit_stat = 1;
        return (1);
    }
    return (0);
}

int change_directory(const char *path, t_shell *shell)
{
    char    *old_pwd;
    char    *new_logical_path;
    char    buffer[1024];

    old_pwd = get_env_value(shell->env, "PWD");
    if (!old_pwd)
        old_pwd = "";  // si PWD n'existe pas, on part d'une base vide
    new_logical_path = build_logical_path(old_pwd, path);
    if (handle_cd_errors(path, shell, new_logical_path))
        return (1);

    if (!getcwd(buffer, sizeof(buffer)))
    {
        print_error("cd", "getcwd failed (directory might be deleted)");
        update_env_vars(&shell->env, new_logical_path, shell);
        free(new_logical_path);
        shell->exit_stat = 0;
        return (0);
    }
    update_env_vars(&shell->env, buffer, shell);
    free(new_logical_path);
    shell->exit_stat = 0;
    return (0);
}

/*
** builtin_cd: construit 'path' (HOME, OLDPWD, ou argument),
** puis appelle change_directory().
*/
int builtin_cd(t_command *cmd, t_shell *shell)
{
    char *path;

    path = NULL;
    if (!cmd->argv[1])
    {
        path = get_env_value(shell->env, "HOME");
        if (!path || !*path)
        {
            print_error("cd", "HOME not set");
            return (1);
        }
    }
    else if (ft_strcmp(cmd->argv[1], "-") == 0)
    {
        path = get_env_value(shell->env, "OLDPWD");
        if (!path || !*path)
        {
            print_error("cd", "OLDPWD not set");
            return (1);
        }
    }
    else
        path = cmd->argv[1];

    return (change_directory(path, shell));
}

/* Vérifie si un argument est une option `-n`, `-e` ou `-E` */
static int	is_valid_echo_option(const char *arg, char opt)
{
	int	i;

	if (!arg || arg[0] != '-' || arg[1] != opt)
		return (0);
	i = 1;
	while (arg[++i])
		if (arg[i] != opt)
			return (0);
	return (1);
}

/* Transforme `\n` et autres séquences si `-e` est actif */
static char	*parse_escape_sequences(char *str, t_shell *shell)
{
	char	*new_str;
	int		i;
	int		j;
		
	i = 0;
	j = 0;
	new_str = safe_malloc(ft_strlen(str) + 1, shell);
	if (!new_str)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\\' && str[i + 1] == 'n')
		{
			new_str[j++] = '\n';
			i += 2;
		}
		else
			new_str[j++] = str[i++];
	}
	new_str[j] = '\0';
	return (new_str);
}

/* Exécution de `echo` */

void	process_echo_options(char **args, int *i, int *newline, int *enable_escape)
{
	while (args[*i])
	{
		if (is_valid_echo_option(args[*i], 'n'))
			*newline = 0;
		else if (is_valid_echo_option(args[*i], 'e'))
			*enable_escape = 1;
		else if (is_valid_echo_option(args[*i], 'E'))
			*enable_escape = 0;
		else
			break ;
		(*i)++;
	}
}

void	write_echo_output(char **args, int i, int enable_escape, t_shell *shell)
{
	char	*output;

	while (args[i])
	{
		output = args[i];
		if (enable_escape)
			output = parse_escape_sequences(args[i], shell);
		if (write(STDOUT_FILENO, output, ft_strlen(output)) == -1)
		{
			print_error("echo", "write error");
			shell->exit_stat = 1;
			return ;
		}
		if (enable_escape)
			free(output);
		if (args[i + 1] && write(STDOUT_FILENO, " ", 1) == -1)
		{
			print_error("echo", "write error");
			shell->exit_stat = 1;
			return ;
		}
		i++;
	}
}

void	handle_echo_newline(int newline, t_shell *shell)
{
	if (newline && write(STDOUT_FILENO, "\n", 1) == -1)
	{
		print_error("echo", "write error");
		shell->exit_stat = 1;
		return ;
	}
	shell->exit_stat = 0;
}

void	builtin_echo(char **args, t_shell *shell)
{
	int	i;
	int	newline;
	int	enable_escape;

	i = 1;
	newline = 1;
	enable_escape = 0;

	process_echo_options(args, &i, &newline, &enable_escape);
	write_echo_output(args, i, enable_escape, shell);
	handle_echo_newline(newline, shell);
}

char	*get_env_value(t_env_var *env, const char *key)
{
	if (!key || !env)
		return (NULL);
	while (env)
	{
		if (ft_strcmp(env->name, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}


/*int	handle_heredoc(t_command *cmd, t_shell *shell)
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
}*/

int	process_heredoc_input(int pipe_fd[2], char *line, t_command *cmd, t_shell *shell, int expansion)
{
	if (ft_strcmp(line, cmd->heredoc_delimiter) == 0)
	{
		free(line);
		return (1); // Stop reading on delimiter
	}
	if (expansion)
	{
		char *expanded = replace_env_variables(line, shell);
		free(line);
		if (!expanded) // Handle expansion failure
		{
			close(pipe_fd[1]);
			return (-1);
		}
		line = expanded;
	}
	if (write(pipe_fd[1], line, ft_strlen(line)) < 0 || write(pipe_fd[1], "\n", 1) < 0)
	{
		free(line);
		close(pipe_fd[1]);
		return (-1);
	}
	free(line);
	return (0); // Continue reading
}

void	handle_heredoc_child(int pipe_fd[2], t_command *cmd, t_shell *shell, int expansion)
{
	char	*line;

	signal(SIGINT, SIG_DFL); // Restore default SIGINT behavior
	close(pipe_fd[0]); // Close read end of the pipe
	while (1)
	{
		line = readline("heredoc> ");
		if (!line) // Handle EOF (Ctrl+D)
		{
			fprintf(stderr, "bash: warning: here-document delimited by end-of-file (wanted `%s')\n", cmd->heredoc_delimiter);
			close(pipe_fd[1]);
			exit(0);
		}
		int result = process_heredoc_input(pipe_fd, line, cmd, shell, expansion);
		if (result == 1) // Delimiter found
			break;
		if (result == -1) // Error occurred
			exit(1);
	}
	close(pipe_fd[1]);
	exit(0);
}

int	handle_heredoc_parent(int pipe_fd[2], pid_t pid, t_command *cmd, t_shell *shell)
{
	int	status;

	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	setup_signals();
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		close(pipe_fd[0]);
		shell->exit_stat = 130;
		return (-1);
	}
	if (!cmd->argv || !cmd->argv[0])
	{
		close(pipe_fd[0]);
		return (-1);
	}
	cmd->pipe_in = pipe_fd[0];
	return (0);
}

int	handle_heredoc(t_command *cmd, t_shell *shell)
{
	int		pipe_fd[2];
	int		expansion;
	pid_t	pid;

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
	if (pid == 0)
		handle_heredoc_child(pipe_fd, cmd, shell, expansion);
	return (handle_heredoc_parent(pipe_fd, pid, cmd, shell));
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
    return (
        ft_strcmp(cmd, "cd") == 0 ||
        ft_strcmp(cmd, "export") == 0 ||
        ft_strcmp(cmd, "unset") == 0 ||
        ft_strcmp(cmd, "exit") == 0 ||
        ft_strcmp(cmd, "pwd") == 0  // ✅ Ajout pour corriger pwd
    );
}

void	execute_commands(t_shell *shell, t_command *commands)
{
	t_command	*cmd;
	int			prev_pipe_fd;
	int			pipe_fd[2];

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
			// Sauvegarde des descripteurs standard
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

			// Exécution du builtin
			shell->exit_stat = execute_builtin(cmd, shell);

			// Restauration des descripteurs standard
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
			pid_t	pid;
			int		status;

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
				g_child_pid = pid; // Stockage du PID de l'enfant actif
				waitpid(pid, &status, 0); // Attente de la fin de l'enfant
				shell->exit_stat = WEXITSTATUS(status); // ✅ Correction ici
				g_child_pid = 0; // Réinitialisation après la fin de l'enfant
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
void	set_env_var(t_env_var **env, char *name, char *value, t_shell *shell)
{
	t_env_var	*var;

	var = env_find_var(*env, name);
	if (var)
	{
		free(var->value);
		var->value = ft_strdup(value);
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

/* 🔥 Vérifie si une chaîne est un nombre valide */
static int	is_valid_number(const char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	while (*str == ' ' || *str == '\t')
		str++;
	if (*str == '-' || *str == '+')
		str++;
	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	if (str[i] != '\0')
		return (0);

	return (1);
}

/* 🔥 Convertit une chaîne en long long avec gestion des erreurs */
static long long	handle_overflow(const char *str, int i, long long result, int sign, int *error)
{
	if (result > (LLONG_MAX / 10))
	{
		*error = 1;
		return (sign == 1 ? LLONG_MAX : LLONG_MIN);
	}
	if (result == (LLONG_MAX / 10) && (str[i] - '0') > (LLONG_MAX % 10))
	{
		*error = 1;
		return (sign == 1 ? LLONG_MAX : LLONG_MIN);
	}
	return (result * 10 + (str[i] - '0'));
}

static long long	ft_atoll(const char *str, int *error)
{
	int			sign;
	long long	result;
	int			i;

	sign = 1;
	result = 0;
	i = 0;
	*error = 0;

	while (str[i] == ' ' || str[i] == '\t')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}

	while (ft_isdigit(str[i]))
	{
		result = handle_overflow(str, i, result, sign, error);
		if (*error)
			return (sign == 1 ? LLONG_MAX : LLONG_MIN);
		i++;
	}

	return (result * sign);
}

/* 🔥 Fonction principale de l'exit */
static int	validate_and_parse_exit_code(char *arg, long long *exit_code, t_shell *shell)
{
	char	*trimmed;
	int		error;

	trimmed = ft_strtrim(arg, " \t");
	if (!trimmed || !is_valid_number(trimmed))
	{
		fprintf(stderr, "minishell: exit: %s: numeric argument required\n", arg);
		free(trimmed);
		shell->exit_stat = 255;
		exit(255);
	}

	*exit_code = ft_atoll(trimmed, &error);
	free(trimmed);

	if (error)
	{
		fprintf(stderr, "minishell: exit: %s: numeric argument required\n", arg);
		shell->exit_stat = 255;
		exit(255);
	}

	return (0);
}

int	builtin_exit(char **args, t_shell *shell)
{
	long long	exit_code;

	if (isatty(STDIN_FILENO))
		printf("exit\n");

	if (args[1])
	{
		if (validate_and_parse_exit_code(args[1], &exit_code, shell) != 0)
			return (1); // Error already handled in validate_and_parse_exit_code

		if (args[2])
		{
			fprintf(stderr, "minishell: exit: too many arguments\n");
			shell->exit_stat = 1;
			return (1);
		}

		exit_code = exit_code % 256;
		exit(exit_code);
	}

	exit(shell->exit_stat);
}

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
		name = safe_strdup(args[1], shell);
	if (!name)
		return (1);
	if (!is_valid_identifier(name))
	{
		print_error("export", "not a valid identifier");
		free(name);
		return (1);
	}
	if (equal_pos)
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
/*
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
*/
/*
** builtin_pwd:
** - On tente getcwd().
** - Si ça échoue, on affiche la valeur de $PWD.
*/
int builtin_pwd(t_shell *shell)
{
    char buffer[1024];
    char *env_pwd;

    if (getcwd(buffer, sizeof(buffer)))
    {
        ft_putstr_fd(buffer, STDOUT_FILENO);
        ft_putstr_fd("\n", STDOUT_FILENO);
        shell->exit_stat = 0;
        return (0);
    }
    // si getcwd échoue, on essaie de récupérer $PWD
    env_pwd = get_env_value(shell->env, "PWD");
    if (env_pwd)
    {
        ft_putstr_fd(env_pwd, STDOUT_FILENO);
        ft_putstr_fd("\n", STDOUT_FILENO);
        shell->exit_stat = 0;
        return (0);
    }
    // si rien n'existe, on affiche "."
    ft_putstr_fd(".\n", STDOUT_FILENO);
    shell->exit_stat = 1;
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
		ret = builtin_pwd(shell);
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
/*
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
*/
int	validate_commands(t_command *commands)
{
	t_command	*cmd;

	cmd = commands;
	while (cmd)
	{
		// Vérifie qu'il y a bien une commande (pas vide)
		if (!cmd->argv || !cmd->argv[0])
		{
			// On autorise un heredoc seul : "<<EOF"
			if (cmd->heredoc)
				return (0);
			print_error("syntax error", "unexpected empty command");
			return (-1);
		}

		// Vérifie qu'après un pipe, on ait une commande suivante
		if (cmd->pipe_out && (!cmd->next || !cmd->next->argv || !cmd->next->argv[0]))
		{
			print_error("syntax error", "near unexpected token '|'");
			return (-1);
		}

		// SUPPRIMER ou COMMENTER la vérification du fichier :
		/*
		if (cmd->input_file && access(cmd->input_file, F_OK) != 0)
		{
			print_error(cmd->input_file, "No such file or directory");
			return (-1);
		}
		*/

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

