#include "minishell.h"

void	free_token(t_token **tokens)
{
	t_token	*cur;
	t_token	*tmp;

	if (!tokens)
		return ;
	cur = *tokens;
	while (cur)
	{
		tmp = cur->next;
		if (cur->str)
			free(cur->str);
		if (cur->envp)
			free(cur->envp);
		free(cur);
		cur = tmp;
	}
	*tokens = NULL;
}

void	free_all_env(t_env **env)
{
	t_env	*cur;
	t_env	*tmp;

	if (!env)
		return ;
	cur = *env;
	while (cur)
	{
		tmp = cur->next;
		if (cur->name)
			free(cur->name);
		if (cur->value)
			free(cur->value);
		free(cur);
		cur = tmp;
	}
	*env = NULL;
}

void	free_all_localvar(t_localvar **localvar)
{
	t_localvar	*cur;
	t_localvar	*tmp;

	if (!localvar)
		return ;
	cur = *localvar;
	while (cur)
	{
		tmp = cur->next;
		if (cur->name)
			free(cur->name);
		if (cur->value)
			free(cur->value);
		free(cur);
		cur = tmp;
	}
	*localvar = NULL;
}
