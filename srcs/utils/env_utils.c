
#include "minishell.h"
#include "exec.h"

t_env	*find_last_exportvar(t_env *node)
{
	if (!node)
		return (NULL);
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

void	set_env_value(t_env *env, char *name, char *new_value) //modifie valeur de name dans env
{
	while (env)
	{
		if (!ft_strcmp(env->name, name))
		{
			free(env->value);
			env->value = ft_strdup(new_value);
			return ;
		}
		env = env->next;
	}
}

void	free_env(t_env *del_env, t_env **env)
{
	if (del_env->next)
	{
		if (del_env->prev)
		{
			del_env->prev->next = del_env->next;
			del_env->next->prev = del_env->prev;
		}
		else
		{
			del_env->next->prev = NULL;
			*env = del_env->next;
		}
	}
	else
	{
		if (del_env->prev)
			del_env->prev->next = NULL;
		else
			*env = NULL;
	}
	free(del_env->name);
	free(del_env->value);
	free(del_env);
}