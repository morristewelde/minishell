/***************************************************************************
** Exemple d'un code pipeline-based
** (Le code n'est pas forcément 100% plug-and-play, mais illustre la logique)
***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

/* ---------- Enum & Struct ---------- */
typedef enum e_token_type
{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIRECT_IN,
    TOKEN_REDIRECT_OUT,
    TOKEN_REDIRECT_APPEND,
    TOKEN_HEREDOC,
    TOKEN_SEMICOLON,
    TOKEN_UNSUPPORTED
} t_token_type;

typedef struct s_token
{
    char            *value;
    t_token_type    type;
    struct s_token  *next;
} t_token;

typedef struct s_command
{
    char            **argv;
    char            *input_file;
    char            *output_file;
    int             append;
    int             heredoc;
    char            *heredoc_delimiter;
    int             pipe_in;
    int             pipe_out;

    t_token         *token_list; // Liste de tokens bruts de la commande
    struct s_command *next;
} t_command;

typedef struct s_pipeline
{
    t_command          *commands; // liste chainée de commands reliées par '|'
    struct s_pipeline  *next;
} t_pipeline;

/* ---------- Prototypes simplifiés ---------- */
t_token     *tokenize_input(const char *input);
t_pipeline  *parse_sequence(t_token *tokens);
t_command   *parse_pipeline(t_token *start, t_token *end);
void        organize_command_tokens(t_command *cmd);
void        execute_all_pipelines(t_pipeline *pipelines);

/* ---------- Fonctions d'Utilité Minimale ---------- */
char *mini_strdup(const char *s)
{
    char *dup = malloc(strlen(s) + 1);
    if (dup) strcpy(dup, s);
    return dup;
}

t_token *new_token(const char *val, t_token_type type)
{
    t_token *tok = calloc(1,sizeof(t_token));
    if (!tok) return NULL;
    tok->value = mini_strdup(val);
    tok->type  = type;
    return tok;
}

void token_add_back(t_token **lst, t_token *newt)
{
    if (!lst || !newt) return;
    if (!*lst) { *lst = newt; return; }
    t_token *cur = *lst;
    while (cur->next) cur = cur->next;
    cur->next = newt;
}

void free_token_list(t_token *lst)
{
    while (lst)
    {
        t_token *n = lst->next;
        free(lst->value);
        free(lst);
        lst = n;
    }
}

/* ---------- 1) Tokenisation Minimale (avec ;, |, <, >, >>, <<) ---------- */
t_token *tokenize_input(const char *input)
{
    /* Version TRÈS SIMPLIFIÉE, on split sur ' ' + on détecte symboles.  
       Pour un vrai code, il faut handle quotes, expansions etc. 
    */
    t_token *head = NULL;

    // Séparation simpliste par espaces
    char *tmp = mini_strdup(input);
    char *tok = strtok(tmp, " \t");
    while (tok)
    {
        // Reconnaissance naive
        if (strcmp(tok,";")==0)
            token_add_back(&head, new_token(tok, TOKEN_SEMICOLON));
        else if (strcmp(tok,"|")==0)
            token_add_back(&head, new_token(tok, TOKEN_PIPE));
        else if (strcmp(tok,">")==0)
            token_add_back(&head, new_token(tok, TOKEN_REDIRECT_OUT));
        else if (strcmp(tok,">>")==0)
            token_add_back(&head, new_token(tok, TOKEN_REDIRECT_APPEND));
        else if (strcmp(tok,"<")==0)
            token_add_back(&head, new_token(tok, TOKEN_REDIRECT_IN));
        else if (strcmp(tok,"<<")==0)
            token_add_back(&head, new_token(tok, TOKEN_HEREDOC));
        else
            token_add_back(&head, new_token(tok, TOKEN_WORD));

        tok = strtok(NULL, " \t");
    }
    free(tmp);
    return head;
}

/* ---------- 2) parse_sequence => scinde sur ';' en t_pipeline ---------- */
t_pipeline *create_pipeline_node(void)
{
    t_pipeline *p = calloc(1,sizeof(t_pipeline));
    return p;
}

t_token *find_semicolon(t_token *toks)
{
    while (toks)
    {
        if (toks->type == TOKEN_SEMICOLON)
            return toks;
        toks = toks->next;
    }
    return NULL;
}

t_pipeline *parse_sequence(t_token *tokens)
{
    t_pipeline *head = NULL, *last = NULL;

    while (tokens)
    {
        // Cherche le prochain `;`
        t_token *seg_end = find_semicolon(tokens);
        t_token *next_start = NULL;

        if (seg_end)
        {
            next_start = seg_end->next;
            seg_end->next = NULL; // coupe la liste
        }

        // Crée un pipeline
        t_pipeline *pnode = create_pipeline_node();
        if (!pnode) return NULL; // mémoire foirée
        // parse_pipeline => scinde sur '|'
        pnode->commands = parse_pipeline(tokens, NULL);

        // Ajoute à la liste chainée
        if (!head) head = pnode;
        else last->next = pnode;
        last = pnode;

        // Restaure la liste si on l'avait coupée
        tokens = next_start; 
    }
    return head;
}

/* ---------- 3) parse_pipeline => scinde sur '|' en t_command ---------- */
t_command *new_command_node(void)
{
    t_command *cmd = calloc(1,sizeof(t_command));
    return cmd;
}

void add_token_to_command(t_command *cmd, t_token *original)
{
    // Copie le token dans cmd->token_list (un champ brut)
    // On alloue un nouveau t_token
    t_token *dup = calloc(1,sizeof(t_token));
    dup->value = mini_strdup(original->value);
    dup->type  = original->type;
    // chainage
    if (!cmd->token_list) cmd->token_list = dup;
    else
    {
        t_token *c = cmd->token_list;
        while(c->next) c = c->next;
        c->next = dup;
    }
}

t_token *find_pipe(t_token *toks)
{
    while (toks)
    {
        if (toks->type == TOKEN_PIPE)
            return toks;
        toks = toks->next;
    }
    return NULL;
}

/* parse_pipeline scinde un "segment" en plusieurs commands reliées par '|'. 
   end peut être NULL => parse jusqu'à la fin */
t_command *parse_pipeline(t_token *start, t_token *end)
{
    t_command *cmd_list = NULL, *last_cmd = NULL;

    while (start && start != end)
    {
        // Cherche le prochain token '|' 
        t_token *pipe_token = find_pipe(start);
        t_token *next_start = NULL;
        if (pipe_token && (pipe_token != end))
        {
            next_start = pipe_token->next;
            pipe_token->next = NULL; // coupe la liste
        }

        // Crée un t_command
        t_command *newcmd = new_command_node();
        if (!newcmd) return cmd_list;

        // Parcourt la liste [start .. pipe_token[ pour l'ajouter 
        t_token *cur = start;
        while(cur)
        {
            add_token_to_command(newcmd, cur);
            cur = cur->next;
        }

        // On relie newcmd
        if (!cmd_list) cmd_list = newcmd;
        else last_cmd->next = newcmd;
        last_cmd = newcmd;

        // Gère pipe_in / pipe_out
        if (pipe_token) 
        {
            newcmd->pipe_out = 1;
            // La commande suivante aura pipe_in = 1
        }

        // Restaure la liste si on l'a coupée
        if (pipe_token)
            start = next_start;
        else
            break;
    }
    return cmd_list;
}

/* ---------- 4) organize_command_tokens => transform token_list -> argv[], etc. ---------- */
#include <fcntl.h>

void organize_command_tokens(t_command *cmd)
{
    while (cmd)
    {
        // On regarde cmd->token_list
        int word_count = 0;
        t_token *t = cmd->token_list;
        while (t)
        {
            if (t->type == TOKEN_WORD)
                word_count++;
            t = t->next;
        }
        // on alloue argv
        cmd->argv = calloc(word_count+1,sizeof(char*));
        // on remplit
        int i=0;
        t = cmd->token_list;
        while(t)
        {
            if (t->type == TOKEN_WORD)
            {
                cmd->argv[i] = mini_strdup(t->value);
                i++;
            }
            else if (t->type == TOKEN_REDIRECT_IN && t->next)
            {
                cmd->input_file = mini_strdup(t->next->value);
                // skip
                t = t->next;
            }
            else if (t->type == TOKEN_REDIRECT_OUT && t->next)
            {
                cmd->output_file = mini_strdup(t->next->value);
                cmd->append = 0;
                t = t->next;
            }
            else if (t->type == TOKEN_REDIRECT_APPEND && t->next)
            {
                cmd->output_file = mini_strdup(t->next->value);
                cmd->append = 1;
                t = t->next;
            }
            else if (t->type == TOKEN_HEREDOC && t->next)
            {
                cmd->heredoc_delimiter = mini_strdup(t->next->value);
                cmd->heredoc = 1;
                t = t->next;
            }
            t = t ? t->next : NULL;
        }

        // pipe_in / pipe_out sera déterminé par parse_pipeline()
        cmd = cmd->next;
    }
}

/* ---------- 5) Exécution des pipelines ---------- */
// Fonctions minimales

int handle_redirections(t_command *cmd)
{
    if (cmd->input_file)
    {
        int fd = open(cmd->input_file,O_RDONLY);
        if (fd<0) {perror("open input"); return -1;}
        dup2(fd,STDIN_FILENO); close(fd);
    }
    if (cmd->output_file)
    {
        int fd;
        if (cmd->append)
            fd = open(cmd->output_file,O_WRONLY|O_CREAT|O_APPEND, 0644);
        else
            fd = open(cmd->output_file,O_WRONLY|O_CREAT|O_TRUNC, 0644);
        if (fd<0){perror("open output"); return -1;}
        dup2(fd,STDOUT_FILENO);close(fd);
    }
    return 0;
}

/* simili builtin check */
int is_builtin(const char *cmd)
{
    if (!cmd) return 0;
    if (strcmp(cmd,"pwd")==0) return 1;
    if (strcmp(cmd,"echo")==0) return 1;
    // ...
    return 0;
}

int do_builtin(t_command *cmd)
{
    if (!strcmp(cmd->argv[0],"pwd"))
    {
        char cwd[1024];
        getcwd(cwd,sizeof(cwd));
        printf("%s\n",cwd);
        return 0;
    }
    if (!strcmp(cmd->argv[0],"echo"))
    {
        int i=1;
        while (cmd->argv[i])
        {
            printf("%s",cmd->argv[i]);
            if (cmd->argv[i+1]) printf(" ");
            i++;
        }
        printf("\n");
        return 0;
    }
    return 1; // inconnu
}

/* mini handle_heredoc */
int handle_heredoc(t_command *cmd)
{
    (void)cmd;
    // TODO: si cmd->heredoc_delimiter => faire un pipe + read. 
    // code de demonstration ...
    return 0;
}

void execute_pipeline(t_command *cmds)
{
    // on fait "while cmds"
    int prev_fd = -1;
    while(cmds)
    {
        int pipefd[2] = {-1,-1};
        if (cmds->pipe_out)
        {
            if (pipe(pipefd) <0){ perror("pipe"); return;}
        }

        pid_t pid = fork();
        if (pid == 0)
        {
            if (prev_fd != -1) // reçois input
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }
            if (cmds->pipe_out) // envoie output
            {
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[0]);
                close(pipefd[1]);
            }
            // gestion redir
            if (handle_redirections(cmds)<0) exit(1);
            if (is_builtin(cmds->argv[0])==1)
            {
                do_builtin(cmds);
                exit(0);
            }
            else
            {
                // exec
                execvp(cmds->argv[0], cmds->argv);
                perror("execvp");
                exit(127);
            }
        }
        else
        {
            // parent
            int status=0;
            if (prev_fd != -1) close(prev_fd);
            if (cmds->pipe_out)
            {
                close(pipefd[1]);
                prev_fd = pipefd[0];
            } 
            else 
                prev_fd = -1;
            waitpid(pid,&status,0);
        }

        cmds = cmds->next;
    }
}

void execute_all_pipelines(t_pipeline *pipelines)
{
    while (pipelines)
    {
        // avant de lancer, on organise command tokens -> argv
        t_command *c = pipelines->commands;
        while (c)
        {
            // handle heredoc
            if (c->heredoc_delimiter)
                handle_heredoc(c);  // pseudo
            // puis "organize_command_tokens" a déjà été fait, 
            // ou on l'appelle avant. (Ici on suppose qu'on l'a déjà fait)
            c = c->next;
        }
        execute_pipeline(pipelines->commands);
        pipelines = pipelines->next;
    }
}

/* ---------- 6) main de demonstration ---------- */
int main(void)
{
    while(1)
    {
        char *input = readline("mypipe> ");
        if (!input) { printf("exit\n"); break; }
        if (*input) add_history(input);

        // token
        t_token *toks = tokenize_input(input);
        free(input);
        if (!toks) continue;

        // parse en pipelines
        t_pipeline *pls = parse_sequence(toks);

        // free la liste globale
        free_token_list(toks);

        // pour chaque pipeline => parse_pipeline => on l’a déjà fait
        // IL NOUS RESTE la phase "organize_command_tokens" (pour argv etc.)
        // On la fait commande par commande:
        t_pipeline *p = pls;
        while (p)
        {
            t_command *c = p->commands;
            while (c)
            {
                organize_command_tokens(c);
                c = c->next;
            }
            p = p->next;
        }

        // exécution
        execute_all_pipelines(pls);

        // free pipeline
        // (Il faut free la token_list, argv[], etc. 
        //  pour la demo on omet par souci de brièveté. 
        //  Cf. free(...) plus complet dans un vrai code.)
        // ...
    }
    return 0;
}

