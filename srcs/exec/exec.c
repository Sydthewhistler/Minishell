
#include "minishell.h"
#include "exec.h"

void	exec_cmd(t_token *token, char **cmd_arg)
{
	pid_t	id;

	id = fork();
	if (id < 0)
		error("fork failed");
	if (id == 0)
	{
		// à potentiellement corriger plus tard : donner un vrai tableau argv et envp
		if (execve(token->envp, cmd_arg, NULL) == -1)
			error("execve failed");
	}
	else
		waitpid(id, NULL, 0);
}

void	redirect(t_token *token, int p_read, int p_write, int print_pipe)
{
	int fd;

	fd = -1;
	if(is_redirectin(token)) // si provient redirection fichier
	{
		fd = open(find_rdin_file(token), O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if(fd == -1)
			error("Error while opening redirectin file");
		dup2(fd, STDIN_FILENO);
	}
	if(is_precededpipe(token)) // si précédé d un pipe
		dup2(p_read, STDIN_FILENO);
	dup2(print_pipe, STDOUT_FILENO); // par defaut rediriger sortie vers pipe pour print apres
	if(is_redirectout(token)) // si va redirection fichier
	{
		fd = open(find_rdout_file(token), O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if(fd == -1)
			error("Error while opening redirectin file");
		dup2(fd, STDOUT_FILENO);
	}
	if(is_followedpipe(token)) // si suivit d un pipe
		dup2(p_write, STDOUT_FILENO);
	if(fd != -1)
		close(fd);
}

void	print_result(int output)
{
	int bytes;
	char buffer[1024];

	while (1)
	{
		bytes = read(output, buffer, sizeof(buffer));
		if(bytes < 1)
			break;
		write(1, buffer, bytes);
	}
}

char **create_cmd(t_token *token)
{
	char **cmd_arg;
	t_token *start;
	int	arg_nb;
	int	i;

	arg_nb = 0;
	start= token;
	token = token->next;
	while(token->role == ROLE_ARGUMENT)
		arg_nb++;
	cmd_arg = malloc(sizeof(char *) * (arg_nb + 1));
	cmd_arg[1] = ft_strdup(start->str);
	start= start->next;
	i = 1;
	while(start->role == ROLE_ARGUMENT)
	{
		cmd_arg[i] = ft_strdup(start->str);
		i++;
	}
	cmd_arg[i] = NULL;
	return (cmd_arg);
}

void	exec(t_token *token, int p_read, int p_write)
{
	int stdout_backup;
	int stdin_backup;
	int print_pipe[2]; // pipe contenant la sortie du execve pour imprimer le resultat
	char **cmd_arg;

	stdout_backup = dup(STDOUT_FILENO);
	stdin_backup = dup(STDIN_FILENO);
	pipe(print_pipe);
	redirect(token,p_read, p_write, print_pipe[1]);
	cmd_arg = create_cmd(token);
	exec_cmd(token, cmd_arg);
	free_tab(cmd_arg);
	print_result(print_pipe[0]);
	close(print_pipe[0]);
	close(print_pipe[1]);
	if(dup(STDOUT_FILENO) != stdout_backup) // si output n est pas standard, le remettre std
		dup2(stdout_backup, STDOUT_FILENO);
	if(dup(STDIN_FILENO) != stdin_backup) // si input n est pas standard, le remettre std
		dup2(stdin_backup, STDIN_FILENO);
}
