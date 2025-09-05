#include "exec.h"
#include "minishell.h"

volatile sig_atomic_t	g_signal = 0;

// static const char *content_types[] = {
//     "UNDEFINED",
//     "WORD",
//     "QUOTED",
//     "OPERATOR",
//     "HEREDOC"
// };

// static const char *role_types[] = {
//     "UNDEFINED",
//     "COMMAND",
//     "ARGUMENT",
//     "FILENAME",
//     "PIPE",
//     "REDIRECT_IN",
//     "REDIRECT_OUT",
//     "REDIRECT_APPEND",
//     "REDIRECT_HEREDOC",
//     "HEREDOC_CONTENT"
// };

// void    debug_print_tokens(t_token *tokens)
// {
//     t_token *current;
//     int     i;

//     i = 0;
//     current = tokens;
//     printf("\n=== DEBUG: TOKEN LIST ===\n");
//     while (current)
//     {
//         printf("Token[%d]:\n", i++);
//         printf("  str   : '%s'\n", current->str ? current->str : "NULL");
//         printf("  type  : %s (%d)\n", 
//             current->type < 5 ? content_types[current->type] : "UNKNOWN",
//             current->type);
//         printf("  role  : %s (%d)\n", 
//             current->role < 10 ? role_types[current->role] : "UNKNOWN",
//             current->role);
//         printf("  envp  : %s\n", current->envp ? current->envp : "NULL");
//         printf("-------------------\n");
//         current = current->next;
//     }
//     printf("=== END TOKEN LIST ===\n\n");
// }

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

static void	process_line(char *line, t_token **tokens, t_shell *shell)
{
	int	parsing_status;

	add_history(line);
	parsing_status = parse_line(line, tokens, shell);
	if (*tokens && parsing_status && ft_strcmp((*tokens)->str, "exit") != 0)
		shell->exit_code = exec_master(*tokens, &(shell->env), shell->localvar);
	check_exit_code(*tokens,shell);
	free_token(tokens);
	*tokens = NULL;
	ft_setup_interactive_signal();
}

static int	handle_signal_interrupt(t_shell *shell, char *line)
{
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
	return (0);
}

static int	handle_shell_iteration(t_shell *shell, t_token **tokens)
{
	char	*line;

	line = readline("minishell>");
	g_signal = 0;
	if (!line)
	{
		printf("exit\n");
		return (0);
	}
	if (handle_signal_interrupt(shell, line))
		return (1);
	if (*line != '\0')
		process_line(line, tokens, shell);
	if (shell->should_exit)
	{
		free(line);
		return (0);
	}
	free(line);
	return (1);
}

int	main(int ac, char **av, char **envp)
{
	t_shell	shell;
	t_token	*tokens;

	(void)ac;
	(void)av;
	tokens = NULL;
	if (init_shell(envp, &shell) == -1)
		return (-1);
	ft_setup_interactive_signal();
	while (!shell.should_exit)
	{
		if (!handle_shell_iteration(&shell, &tokens))
			break ;
	}
	rl_clear_history();
	free_all_env(&(shell.env));
	free_all_localvar(shell.localvar);
	free(shell.localvar);
	return (shell.exit_code);
}
