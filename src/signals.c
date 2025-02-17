#include "../include/minishell.h"

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

