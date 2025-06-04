
#include "minishell.h"

char	*joincmd(char const *s1, char const *s2)
{
	unsigned int	i;
	unsigned int	j;
	char			*new_s;

	if (!s1 || !s2)
		return (NULL);
	new_s = malloc(ft_strlen(s1) + ft_strlen(s2) + 2);
	if (!new_s)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		new_s[i] = s1[i];
		i++;
	}
	new_s[i] = ' '; // rajout espace separant cmd et argument
	i++;
	j = 0;
	while (s2[j])
	{
		new_s[i + j] = s2[j];
		j++;
	}
	new_s[i + j] = '\0';
	return (new_s);
}

void	exec_cmd(t_token *token)
{
	pid_t	id;

	id = fork();
	if (id < 0)
		error("fork failed");
	if (id == 0)
	{
		// à potentiellement corriger plus tard : donner un vrai tableau argv et envp
		if (execve(token->envp, token->str, NULL) == -1)
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

void	exec(t_token *token, int p_read, int p_write)
{
	int stdout_backup;
	int stdin_backup;
	int print_pipe[2]; // pipe contenant la sortie du execve pour imprimer le resultat
	//char *cmd; pas utile si coco donne direct la commande colle ex : "ls -la"

	stdout_backup = dup(STDOUT_FILENO);
	stdin_backup = dup(STDIN_FILENO);
	pipe(print_pipe);
	redirect(token,p_read, p_write, print_pipe[1]);
	/*if(token->next->role == ROLE_ARGUMENT) // si commande besion argument, les rentrer
		cmd = joincmd(token->str, token->next->str);
	else
		cmd = ft_strdup(token->str); // sinon envoie juste la commande */
	exec_cmd(token);
	//free(cmd);
	print_result(print_pipe[0]);
	close(print_pipe[0]);
	close(print_pipe[1]);
	if(dup(STDOUT_FILENO) != stdout_backup) // si output n est pas standard, le remettre std
		dup2(stdout_backup, STDOUT_FILENO);
	if(dup(STDIN_FILENO) != stdin_backup) // si input n est pas standard, le remettre std
		dup2(stdin_backup, STDIN_FILENO);
}
