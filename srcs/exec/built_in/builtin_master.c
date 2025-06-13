
#include "minishell.h"
#include "exec.h"

bool	is_builtin(t_token *token)
{
	if (!ft_strcmp(token->str, "echo")) // gere aussi option -n
		return (true);
	else if (!ft_strcmp(token->str, "cd"))
		return (true);
	else if (!ft_strcmp(token->str, "pwd"))
		return (true);
	else if (!ft_strcmp(token->str, "export"))
		return (true);
	else if (!ft_strcmp(token->str, "unset"))
		return (true);
	else if (!ft_strcmp(token->str, "env"))
		return (true);
	else if(ft_contains(token->str, "=")) // si declaration nouvelle localvar
		return (true);
	return (false);
}

void	which_built_in(t_token *token, t_env **env, t_localvar **localvar)
{
	if (!ft_strcmp(token->str, "echo") || !ft_strcmp(token->str, "echo -n")) // gere aussi option -n
		ft_echo(token);
	else if (!ft_strcmp(token->str, "cd"))
		ft_cd(token, *env);
	else if (!ft_strcmp(token->str, "pwd"))
		ft_pwd(*env);
	else if (!ft_strcmp(token->str, "export"))
		ft_export(env, token, localvar);
	else if (!ft_strcmp(token->str, "unset"))
		ft_unset(localvar, env, token);
	else if (!ft_strcmp(token->str, "env"))
		ft_env(*env);
	else if (ft_contains(token->str, "="))
		ft_localvar(localvar, token);
}

int	ft_builtin(t_token *token, t_env **env, t_localvar **localvar, int pipe)
{
	int fd;
	int stdout_backup;

	stdout_backup = dup(STDOUT_FILENO);
	if(is_redirectout(token)) // si redirection dans un fichier
	{
		fd = open(token->next->next->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		dup2(fd, STDOUT_FILENO);
	}
	if(is_followedpipe(token)) // si suivit d un pipe
		dup2(pipe, STDOUT_FILENO);
	which_built_in(token, env, localvar);
	if(dup(STDOUT_FILENO) != stdout_backup)
		dup2(stdout_backup, STDOUT_FILENO);
	return (0);
}