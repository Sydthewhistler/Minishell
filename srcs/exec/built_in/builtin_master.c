
#include "minishell.h"

bool	is_builtin(t_token *token)
{
	if (ft_strchr(token->str, "echo")) // strchr car peut etre suivi de -n
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
	return (false);
}

void	which_built_in(t_token *token, t_env **env, t_localvar **localvar)
{
	if (ft_strchr(token->str, "echo")) // strchr car peut etre suivi de -n
		ft_echo(token->str, token);
	else if (!ft_strcmp(token->str, "cd"))
		ft_cd(token->str);
	else if (!ft_strcmp(token->str, "pwd"))
		ft_pwd();
	else if (!ft_strcmp(token->str, "export"))
		ft_export(env, token, localvar);
	else if (!ft_strcmp(token->str, "unset"))
		ft_unset();
	else if (!ft_strcmp(token->str, "env"))
		ft_env(*env);
}

int	ft_builtin(t_token *token, t_env **env, t_localvar **localvar, int pipe)
{
	int fd;
	int stdout_backup;

	stdout_backup = dup(STDOUT_FILENO);
	if(token->next->role == ROLE_REDIRECT_OUT) // si redirection dans un fichier
	{
		fd = open(token->next->next->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		dup2(fd, STDOUT_FILENO);
	}
	if(token->next->role == ROLE_PIPE) // si suivit d un pipe
		dup2(pipe, STDOUT_FILENO);
	which_built_in(token, env, localvar);
	if(dup(STDOUT_FILENO) != stdout_backup)
		dup2(stdout_backup, STDOUT_FILENO);
	return (0);
}