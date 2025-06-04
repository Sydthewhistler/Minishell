
#include "minishell.h"

void	ft_cd(t_token *token)
{
	char *path;

	if (token->next && token->next->str)
		path = token->next->str;
	else
		path = //valeur HOME dans env 
	if (!path)
	{
		write(2, "cd: HOME not set\n", 18);
		return;
	}
	if (chdir(path) != 0)
	{
		putstr_error("cd: no such file or directory: \0");
		putstr_error(path);
	}
	// remplacer PWD dans env
}
