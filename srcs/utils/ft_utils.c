
#include "minishell.h"

char	*ft_strndup(const char *str, int n)
{
	char	*str2;
	int		i;

	if (!str || n < 0)
		return (NULL);
	str2 = malloc(sizeof(char) * (n + 1));
	if (!str2)
		return (NULL);
	i = 0;
	while (i < n && str[i])
	{
		str2[i] = str[i];
		i++;
	}
	str2[i] = '\0';
	return (str2);
}

void	free_localvar(t_localvar *local, t_localvar **localvar)
{
	if (local->next)
	{
		if (local->prev)
		{
			local->prev->next = local->next;
			local->next->prev = local->prev;
		}
		else
		{
			local->next->prev = NULL;
			*localvar = local->next;
		}
	}
	else
	{
		if (local->prev)
			local->prev->next = NULL;
		else
			*localvar = NULL;
	}
	free(local->name);
	free(local->value);
	free(localvar);
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
