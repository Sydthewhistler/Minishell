
#include "minishell.h"
#include "exec.h"

bool is_follow_flag(t_token *token)
{
	if(token->next && token->next->str[0] == '-')
		return (true);
	return (false);
}

bool	is_builtin(t_token *token)
{
	if ((!ft_strcmp(token->str, "echo") && !is_follow_flag(token)) || (!ft_strcmp(token->str, "echo") && !ft_strcmp(token->next->str, "-n"))) // gere aussi option -n
		return (true);
	if (!ft_strcmp(token->str, "cd") && !is_follow_flag(token))
		return (true);
	else if (!ft_strcmp(token->str, "pwd") && !is_follow_flag(token))
		return (true);
	else if (!ft_strcmp(token->str, "export") && !is_follow_flag(token))
		return (true);
	else if (!ft_strcmp(token->str, "unset") && !is_follow_flag(token))
		return (true);
	else if (!ft_strcmp(token->str, "env") && !is_follow_flag(token))
		return (true);
	else if(ft_contains(token->str, "=")) // si declaration nouvelle localvar
		return (true);
	return (false);
}

void	which_built_in(t_token *token, t_env **env, t_localvar **localvar)
{
	if ((!ft_strcmp(token->str, "echo") && !is_follow_flag(token)) || (!ft_strcmp(token->str, "echo") && !ft_strcmp(token->next->str, "-n"))) // gere aussi option -n
		ft_echo(token);
	if (!ft_strcmp(token->str, "cd") && !is_follow_flag(token))
		ft_cd(token, *env);
	else if (!ft_strcmp(token->str, "pwd") && !is_follow_flag(token))
		ft_pwd(*env, token);
	else if (!ft_strcmp(token->str, "export") && !is_follow_flag(token))
		ft_export(env, token, localvar);
	else if (!ft_strcmp(token->str, "unset") && !is_follow_flag(token))
		ft_unset(localvar, env, token);
	else if (!ft_strcmp(token->str, "env") && !is_follow_flag(token))
		ft_env(*env);
	else if (ft_contains(token->str, "="))
		ft_localvar(localvar, token);
}

int	ft_builtin(t_token *token, t_env **env, t_localvar **localvar, int p_write)
{
	int fd;
	int stdout_backup;

	fd = -1;
	stdout_backup = dup(STDOUT_FILENO);
	if(is_redirectout(token)) // si redirection dans un fichier
	{
		fd = open(ft_filename(token), O_WRONLY | O_CREAT | O_TRUNC, 0644);
		dup2(fd, STDOUT_FILENO);
		printf("test: dup2 effectue\n");
	}
	else if(is_followedpipe(token)) // si suivit d un pipe
		dup2(p_write, STDOUT_FILENO);
	which_built_in(token, env, localvar);
	if(fd != -1)
		close(fd);
	if(p_write != -1)
		close(p_write);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdout_backup);
	return (0);
}
