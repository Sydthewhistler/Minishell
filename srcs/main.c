
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

// Initialise les variables du shell
static int	init_shell(char **envp, t_env **env, t_localvar ***localvar)
{
	if (!envp)
	{
		printf("Error : no env");
		return (-1);
	}
	*localvar = malloc(sizeof(t_localvar *));
	if (!*localvar)
	{
		printf("malloc issue in init_localvar");
		return (-1);
	}
	**localvar = NULL;
	*env = init_env_from_envp(envp);
	rl_readline_name = "minishell";
	return (0);
}

// Traite une ligne de commande
static void	process_line(char *line, t_token **tokens, t_env **env,
		t_localvar **localvar)
{
	int	parsing_status;

	add_history(line);
	parsing_status = parse_line(line, tokens, *env, *localvar);
	if (*tokens && parsing_status)
		g_signal = exec_master(*tokens, env, localvar);
	free_token(tokens);
	*tokens = NULL;
}

// Boucle principale du shell
static void	shell_loop(t_env **env, t_localvar **localvar)
{
	char	*line;
	t_token	*tokens;

	tokens = NULL;
	while (1)
	{
		line = readline("minishell>");
		g_signal = 0;
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (g_signal == SIGNAL_INTERRUPTED)
		{
			//g_signal = 0;
			free(line);
			continue ;
		}
		if (ft_strcmp(line, "exit") == 0)
		{
			free(line);
			break ;
		}
		if (*line != '\0')
			process_line(line, &tokens, env, localvar);
		else
			g_signal = 0;
		free(line);
	}
}

// Nettoie la mémoire à la fin
static void	cleanup_shell(t_env **env, t_localvar **localvar)
{
	rl_clear_history();
	free_all_env(env);
	free_all_localvar(localvar);
	free(localvar);
}

// MAIN simplifié
int	main(int ac, char **av, char **envp)
{
	t_env		*env;
	t_localvar	**localvar;

	env = NULL;
	localvar = NULL;
	(void)ac;
	(void)av;
	if (init_shell(envp, &env, &localvar) == -1)
		return (-1);
	setup_interactive_signals();
	shell_loop(&env, localvar);
	cleanup_shell(&env, localvar);
	return (0);
}
