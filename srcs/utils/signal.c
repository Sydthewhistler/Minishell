#include "exec.h"
#include "minishell.h"

/*void	handle_sigint(int sig)
{
	(void)sig;
	//g_signal = SIGINT;
	g_signal = SIGNAL_INTERRUPTED;  // Met 130 pour Ctrl+C (différent de syntax error)
    write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}
static void	child_sigint_to_eof(int sig)
{
    (void)sig;
    close(STDIN_FILENO); // Simule un Ctrl-D
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

    sa_int.sa_handler = child_sigint_to_eof;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    sigaction(SIGINT, &sa_int, NULL);

    sa_quit.sa_handler = SIG_DFL;
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = 0;
    sigaction(SIGQUIT, &sa_quit, NULL);
}*/

void    ft_sigint_handler(int sig)
{
    (void)sig;
    g_signal = SIGINT;
    write(1, "^C\n", 3);
}

void    ft_sigquit_handler(int sig)
{
    (void)sig;
}

void    ft_setup_signal(void)
{
    struct sigaction    sa;

    ft_memset(&sa, 0, sizeof(sa));
    sa.sa_handler = ft_sigint_handler;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    sa.sa_handler = ft_sigquit_handler;
    sigaction(SIGQUIT, &sa, NULL);
    sa.sa_flags = SA_RESTART;
}
