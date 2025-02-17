#include "../include/minishell.h"

int g_child_pid = 0; // Variable globale pour les signaux

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;

	// 1) Initialisation du shell (copie env, etc.)
	init_shell(&shell, envp);
	//printf("[DEBUG] Shell initialized successfully.\n");

	// 2) Boucle principale
	while (shell.running)
	{
		// 3) Traitement d'un input utilisateur (et exécution)
		process_input(&shell);
	}

	// 4) Nettoyage des ressources
	//printf("[DEBUG] Cleaning up resources.\n");
	free_env(shell.env);
	//printf("[DEBUG] Shell exited successfully.\n");

	// 5) Retourner le code d'erreur / statut
	return (shell.exit_stat);
}
