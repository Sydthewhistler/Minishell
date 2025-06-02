
#include "minishell.h"

void	exec(t_token *token, int p_read, int p_write)
{
	int fd;
	int stdout_backup;
	int stdin_backup;

	stdout_backup = dup(STDOUT_FILENO);
	stdin_backup = dup(STDIN_FILENO);
	if(/*  ft_is precede de redirect in*/) // si redirection dans un fichier
	{
		fd = open(/*precedant ROLE_FILE*/, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		dup2(fd, STDOUT_FILENO);
	}
	if(/*ft_is precede d un pipe*/) // si suivit d un pipe
		dup2(pipe, STDOUT_FILENO);
	if(/*  ft_is suivi de redirect out*/) // si redirection dans un fichier
	{
		fd = open(/* ROLE_FILE suivant*/, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		dup2(fd, STDOUT_FILENO);
	}
	if(/*ft_is suivi d un pipe*/) // si suivit d un pipe
		dup2(pipe, STDOUT_FILENO);

	/*ft_exec contenant l execve*/

	if(dup(STDOUT_FILENO) != stdout_backup)
		dup2(stdout_backup, STDOUT_FILENO);
}