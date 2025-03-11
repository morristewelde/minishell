/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 15:25:47 by sruzzi            #+#    #+#             */
/*   Updated: 2025/03/07 15:26:02 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

/* Copie les variables d'environnement en une liste chaînée */
t_env_var	*copy_env(char **envp, t_shell *shell)
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
		// Trouve le '=' et copie name/value
		char *eq = ft_strchr(envp[i], '=');
		if (!eq)
			continue ;
		name = ft_strndup(envp[i], eq - envp[i]);
		value = safe_strdup(eq + 1, shell);
		if (!name || !value)
			return (NULL);
		new_var = env_new_var(name, value, shell);
		env_add_var(&env_list, new_var);
		free(name);
		free(value);
		i++;
	}
	return (env_list);
}

void	disable_echoctl(void)
{
	struct termios	term;

	if (!isatty(STDIN_FILENO)) // 🔥 Vérifie si on est bien dans un terminal
		return ;
	if (tcgetattr(STDIN_FILENO, &term) == -1)
	{
		perror("tcgetattr");
		return ;
	}
	term.c_lflag &= ~(ECHOCTL);
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
		perror("tcsetattr");
}
/* Initialise le shell avec les variables d'environnement */
/*
void	init_shell(t_shell *shell, char **envp)
{
	shell->env = copy_env(envp, shell);
	if (!shell->env)
	{
		perror("Failed to initialize environment");
		exit(EXIT_FAILURE);
	}
	// Supprimer la valeur d'OLDPWD au démarrage
	//unset_env_var(&shell->env, "OLDPWD");
	shell->exit_stat = 0; // Initialisation du statut de sortie
	shell->running = 1;   // Le shell est actif
}

void	init_shell(t_shell *shell, char **envp)
{
	shell->env = copy_env(envp, shell);
	if (!shell->env)
	{
		perror("Failed to initialize environment");
		cleanup_shell(shell, NULL, NULL, NULL);
		exit(EXIT_FAILURE);
	}

	// Remplace OLDPWD par une chaîne vide (au lieu de le supprimer complètement)
	set_env_var(&shell->env, "OLDPWD", "", shell);

	shell->exit_stat = 0; // Initialisation du statut de sortie
	shell->running = 1;   // Le shell est actif
}
*/
void	init_shell(t_shell *shell, char **envp)
{
	shell->env = copy_env(envp, shell);

	if (!shell->env && envp && envp[0])
	{
		perror("Failed to initialize environment");
		cleanup_shell(shell, NULL, NULL, NULL);
		exit(EXIT_FAILURE);
	}

	// Remplace OLDPWD par une chaîne vide (au lieu de le supprimer complètement)
	set_env_var(&shell->env, "OLDPWD", "", shell);

	shell->exit_stat = 0;
	shell->running = 1;
}

