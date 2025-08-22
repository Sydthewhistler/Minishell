#include "exec.h"
#include "minishell.h"

void	ft_sigint_interactive_handler(int sig)
{
	(void)sig;
	g_signal = SIGINT;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	ft_sigquit_handler(int sig)
{
	(void)sig;
}

void	ft_setup_interactive_signal(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = ft_sigint_interactive_handler;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = ft_sigquit_handler;
	sigaction(SIGQUIT, &sa, NULL);
	sa.sa_flags = SA_RESTART;
}

void	ft_sigint_execution_handler(int sig)
{
	(void)sig;
	g_signal = SIGINT;
	write(1, "\n", 1);
}

void	ft_setup_execution_signal(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = ft_sigint_execution_handler;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = ft_sigquit_handler;
	sigaction(SIGQUIT, &sa, NULL);
	sa.sa_flags = SA_RESTART;
}
