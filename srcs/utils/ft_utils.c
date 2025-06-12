
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

bool	ft_contains(char *haystack, char *needle)
{
	int	i;
	int	j;

	if (!needle || !needle[0])
		return (true);
	i = 0;
	while (haystack[i])
	{
		j = 0;
		while (haystack[i + j] && needle[j] && haystack[i + j] == needle[j])
			j++;
		if (!needle[j])
			return (true);
		i++;
	}
	return (false);
}

char	*ft_strtok(char *str, const char sep)
{
	static char	*save;
	char		*start;

	if (str)
		save = str;
	if (!save || !*save)
		return (0);
	while (*save == sep)
		save++;
	start = save;
	while (*save && *save != sep)
		save++;
	if (*save)
	{
		*save = '\0';
		save++;
	}
	else
		save = 0;
	return (start);
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
