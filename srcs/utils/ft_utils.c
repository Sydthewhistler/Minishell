/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scavalli <scavalli@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:53:13 by scavalli          #+#    #+#             */
/*   Updated: 2025/05/28 15:24:40 by scavalli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strndup(char *str, int n)
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

void	free_localvar(t_localvar *localvar)
{
	if(local->next)
	{
		if(local->prev)
		{
			local->prev->next = local->next;
			local->next->prev = local->prev;
		}
		else
			local->next->prev = NULL;
	}
	else
	{
		if(local->prev)
			local->prev->next = NULL;
		else
			*localvar = NULL;
	}
	free(localvar->name);
	free(localvar->value);
	free(localvar);
}

void	free_env(t_env *env)
{
	if(env->next)
	{
		if(env->prev)
		{
			env->prev->next = env->next;
			env->next->prev = env->prev;
		}
		else
			env->next->prev = NULL;
	}
	else
	{
		if(env->prev)
			env->prev->next = NULL;
		else
			*env = NULL;
	}
	free(env->name);
	free(env->value);
	free(env);
}