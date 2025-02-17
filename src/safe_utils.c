#include "../include/minishell.h"

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

	if (!s)
        return (NULL);
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
