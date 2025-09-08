
#include "exec.h"
#include "minishell.h"

bool	change_directory(char *path)
{
	if (!path)
	{
		putstr_error("cd: HOME not set\n");
		return false;
	}
	if (chdir(path) != 0)
	{
		putstr_error("cd: no such file or directory: ");
		putstr_error(path);
		write(2, "\n", 1);
		return false;
	}
	return true;
}

void	ft_cd(t_token *token, t_env *env)
{
	char	*path;

	if (token->next && token->next->next
		&& token->next->next->role == ROLE_ARGUMENT)
	{
		putstr_error("cd: trop d'arguments\n");
		return ;
	}
	if (token->next && token->next->str && ft_strcmp(token->next->str,
				"~") != 0)
	{
		path = token->next->str;
		if (!ft_strcmp(path, "."))
			return ;
	}
	else
		path = return_env_value("HOME", env);
	if (change_directory(path))
		update_pwd(env, path);
}
