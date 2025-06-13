/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scavalli <scavalli@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:51:30 by scavalli          #+#    #+#             */
/*   Updated: 2025/06/13 15:54:31 by scavalli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

t_env	*is_env(t_env *env, char *name) // regarde si une variable "name" est une variable exporte
{
	if(!env)
		return (NULL);
	while(env)
	{
		if(ft_strcmp(env->name, name) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

void	ft_env(t_env *env) //affiche var environnement (exportées)
{
	while (env)
	{
		printf("%s=%s\n", env->name, env->value);
		env = env->next;
	}
	return ;
}