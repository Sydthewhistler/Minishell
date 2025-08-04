
#include "exec.h"
#include "minishell.h"

volatile sig_atomic_t	g_signal = 0;

// void	print_list(t_token *tokens)
// {
// 	t_token	*cur;

// 	cur = tokens;
// 	while (cur)
// 	{
// 		printf("token->str : %s \n", cur->str);
// 		printf("token->type : %d \n", cur->type);
// 		printf("token->role : %d \n", cur->role);
// 		printf("token->path : %s \n", cur->envp);
// 		cur = cur->next;
// 	}
// }

void	free_token(t_token **tokens)
{
	t_token	*cur;
	t_token	*tmp;

	if (!tokens)
		return ;
	cur = *tokens;
	while (cur)
	{
		tmp = cur->next;
		if (cur->str)
			free(cur->str);
		if (cur->envp)
			free(cur->envp);
		free(cur);
		cur = tmp;
	}
	*tokens = NULL;
}

void	free_all_env(t_env **env)
{
	t_env	*cur;
	t_env	*tmp;

	if (!env)
		return ;
	cur = *env;
	while (cur)
	{
		tmp = cur->next;
		if (cur->name)
			free(cur->name);
		if (cur->value)
			free(cur->value);
		free(cur);
		cur = tmp;
	}
	*env = NULL;
}

void	free_all_localvar(t_localvar **localvar)
{
	t_localvar	*cur;
	t_localvar	*tmp;

	if (!localvar)
		return ;
	cur = *localvar;
	while (cur)
	{
		tmp = cur->next;
		if (cur->name)
			free(cur->name);
		if (cur->value)
			free(cur->value);
		free(cur);
		cur = tmp;
	}
	*localvar = NULL;
}

int	main(int ac, char **av, char **envp)
{
	char		*line;
	t_token		*tokens;
	t_env		*env;
	t_localvar	**localvar;
	int			parsing_status;

	tokens = NULL;
	env = NULL;
	localvar = NULL;
	(void)ac;
	(void)av;
	if (!envp)
	{
		printf("Error : no env");
		return (-1);
	}
	localvar = malloc(sizeof(t_localvar *));
	if (!localvar)
		error("malloc issue in init_localvar");
	*localvar = NULL;
	env = init_env_from_envp(envp); // creer la liste chainee d env
	rl_readline_name = "minishell"; // juste pour l appeler mnishell
	while (1)
	{
		line = readline("minishell>"); // affiche minishell> et recup la line
		// g_signal = 0;                  // Reset signal
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (g_signal == SIGNAL_INTERRUPTED)
		{
			g_signal = 0; // reset pour nouvelle commande
			free(line);
			continue ; // Nouvelle ligne prompt
		}
		if (ft_strcmp(line, "exit") == 0) // pour sortir taper exit
		{
			free(line);
			break ;
		}
		if (*line != '\0')
		{
			add_history(line);
			parsing_status = parse_line(line, &tokens, env, *localvar);
			if (tokens && parsing_status) // Gardez votre condition originale
			{
				g_signal = exec_master(tokens, &env, localvar);
			}
			free_token(&tokens);
			tokens = NULL;
		}
		else
			g_signal = 0;
		free(line);
	}
	rl_clear_history();
	free_all_env(&env);
	free_all_localvar(localvar);
	free(localvar);
	return (0);
}
