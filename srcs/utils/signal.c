#include "exec.h"
#include "minishell.h"

void	handle_sigint(int sig)
{
	(void)sig;
	//g_signal = SIGINT;
	g_signal = SIGNAL_INTERRUPTED;  // Met 130 pour Ctrl+C (différent de syntax error)
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	 rl_redisplay();
}

void	handle_execution_sigint(int sig)
{
	(void)sig;
	g_signal = SIGNAL_INTERRUPTED;  // Met 130 pour Ctrl+C (différent de syntax error)
	write(STDOUT_FILENO, "\n", 1);
}

void	setup_interactive_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	// Configuration SIGINT
	sa_int.sa_handler = handle_sigint;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
	// Configuration SIGQUIT (ignoré en mode interactif)
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void	setup_execution_signals(void)
{
	struct sigaction sa_int;
	struct sigaction sa_quit;

	// Pendant exécution, SIGINT/SIGQUIT agissent normalement
	sa_int.sa_handler = handle_execution_sigint;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);

	sa_quit.sa_handler = SIG_DFL;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}