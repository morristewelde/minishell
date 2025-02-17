#include "../include/minishell.h"

/* Retourne la couleur du prompt en fonction du statut de la dernière commande */
const char	*get_prompt_color(int last_exit_status)
{
	if (last_exit_status == 0)
		return ("\001" GREEN "\002");
	return ("\001" RED "\002");
}

/* Construit dynamiquement le prompt du shell */
char	*get_prompt(t_shell *shell)
{
	const char	*color;
	const char	*base_prompt;
	char		*final_prompt;

	color = get_prompt_color(shell->exit_stat);
	base_prompt = "minishell> ";

	if (!color || !base_prompt)
	{
		print_error("get_prompt", "Invalid color or base_prompt");
		return (NULL);
	}

	final_prompt = ft_strjoin_three(color, base_prompt, "\001" COLOR_RESET "\002", shell);
	if (!final_prompt)
	{
		print_error("get_prompt", "Failed to allocate memory for prompt");
		return (NULL);
	}
	return (final_prompt);
}
