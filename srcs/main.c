
#include "exec.h"
#include "minishell.h"

volatile sig_atomic_t	g_signal = 0;

// Fonction 1: Initialise la structure shell (25 lignes max)
static int	init_shell(char **envp, t_shell *shell)
{
	if (!envp)
	{
		printf("Error : no env");
		return (-1);
	}
	shell->localvar = malloc(sizeof(t_localvar *));
	if (!shell->localvar)
	{
		printf("malloc issue in init_localvar");
		return (-1);
	}
	*(shell->localvar) = NULL;
	shell->env = init_env_from_envp(envp);
	shell->exit_code = SUCCESS;
	shell->should_exit = 0;
	rl_readline_name = "minishell";
	return (0);
}

// Fonction 2: Traite une ligne de commande (12 lignes)
static void	process_line(char *line, t_token **tokens, t_shell *shell)
{
	int	parsing_status;

	add_history(line);
	parsing_status = parse_line(line, tokens, shell);
	if (*tokens && parsing_status)
		shell->exit_code = exec_master(*tokens, &(shell->env), shell->localvar);
	free_token(tokens);
	*tokens = NULL;
	ft_setup_interactive_signal();
}

// Fonction 3: Gère les signaux et traite une ligne (25 lignes max)
static int	handle_shell_iteration(t_shell *shell, t_token **tokens)
{
	char	*line;

	line = readline("minishell>");
	if (!line)
	{
		printf("exit\n");
		return (0);
	}
	if (g_signal == SIGINT)
	{
		shell->exit_code = SIGNAL_INTERRUPTED;
		g_signal = 0;
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		free(line);
		return (1);
	}
	if (ft_strcmp(line, "exit") == 0)
	{
		shell->should_exit = 1;
		free(line);
		return (0);
	}
	if (*line != '\0')
		process_line(line, tokens, shell);
	free(line);
	return (1);
}

// Fonction 4: Boucle principale du shell (11 lignes)
static void	shell_loop(t_shell *shell)
{
	t_token	*tokens;

	tokens = NULL;
	ft_setup_interactive_signal();
	while (!shell->should_exit)
	{
		if (!handle_shell_iteration(shell, &tokens))
			break ;
	}
}

// Fonction 5: MAIN principal (16 lignes)
int	main(int ac, char **av, char **envp)
{
	t_shell shell;

	(void)ac;
	(void)av;
	if (init_shell(envp, &shell) == -1)
		return (-1);
	shell_loop(&shell);
	rl_clear_history();
	free_all_env(&(shell.env));
	free_all_localvar(shell.localvar);
	free(shell.localvar);
	return (shell.exit_code);
}
