#include "../include/minishell.h"

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

