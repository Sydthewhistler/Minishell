
#include "minishell.h"

t_env	*find_last_exportvar(t_env *node)
{
	if (!node)
		return ;
	while (node->next)
		node = node->next;
	return (node);
}

char *return_env_value(const char *name, const t_env *env)
{
	while(env)
	{
		if(!ft_strcmp(env->name, name))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

t_env	*find_env_var(t_env *env, const char *name)
{
	while (env)
	{
		if (env->name && !ft_strcmp(env->name, name))
			return (env);
		env = env->next;
	}
	return (NULL);
}