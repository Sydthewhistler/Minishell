
#include "minishell.h"
#include "exec.h"

char **create_cmd(t_token *token)
{
	char **cmd_arg;
	t_token *start;
	int	arg_nb;
	int	i;

	arg_nb = 1;
	start = token;
	token = token->next;
	while(token && token->role == ROLE_ARGUMENT)
	{
		arg_nb++;
		token = token->next;
	}
	cmd_arg = malloc(sizeof(char *) * (arg_nb + 1));
	if (!cmd_arg)
		return (NULL);
		
	cmd_arg[0] = ft_strdup(start->str);
	start = start->next;
	i = 1;
	while(start && start->role == ROLE_ARGUMENT)
	{
		cmd_arg[i] = ft_strdup(start->str);
		i++;
		start = start->next;
	}
	cmd_arg[i] = NULL;
	return (cmd_arg);
}
void	redirect(t_token *token, int p_read, int p_write)
{
	int fd;

	fd = -1;
	if(is_redirectin(token)) // si provient redirection fichier
	{
		fd = open(find_rdin_file(token), O_WRONLY | O_CREAT | O_TRUNC, 0644);
		dup2(fd, STDIN_FILENO);
	}
	if(is_precededpipe(token))// si précédé d un pipe
		dup2(p_read, STDIN_FILENO);
	if(is_redirectout(token)) // si va redirection fichier
	{
		fd = open(find_rdout_file(token), O_WRONLY | O_CREAT | O_TRUNC, 0644);
		dup2(fd, STDOUT_FILENO);
	}
	if(is_followedpipe(token))// si suivit d un pipe
		dup2(p_write, STDOUT_FILENO);
	if(fd != -1)
		close(fd);
	if(p_read != -1)
		close(p_read);
	if(p_write != -1)
		close(p_write);
}

void	exec_cmd(t_token *token, char **env_arg, int p_read, int p_write)
{
	pid_t	id;
	char **cmd_arg;

	cmd_arg = create_cmd(token);
	id = fork();
	if (id < 0)
		error("fork failed");
	if (id == 0)
	{
		redirect(token,p_read, p_write);
		if (execve(token->envp, cmd_arg, env_arg) == -1)
			error("execve failed");
	}
	else
		waitpid(id, NULL, 0);
	free_tab(cmd_arg);
}

char	**create_envarg(t_env *env)
{
	char	**envp;
	char	*line;
	int		count = 0;
	t_env	*tmp = env;

	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	count = 0;
	while (env)
	{
		line = ft_strjoin(env->name, "=");
		envp[count++] = ft_strjoin(line, env->value);
		free(line);
		env = env->next;
	}
	envp[count] = NULL;
	return (envp);
}

void	exec(t_token *token, t_env *env, int p_read, int p_write)
{
	char **env_arg;

	env_arg = create_envarg(env);
	exec_cmd(token, env_arg, p_read, p_write);
	free_tab(env_arg);
}
