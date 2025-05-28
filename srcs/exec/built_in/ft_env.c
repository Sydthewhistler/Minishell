/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scavalli <scavalli@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:51:30 by scavalli          #+#    #+#             */
/*   Updated: 2025/05/28 15:19:00 by scavalli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_localvar	*is_env(t_env *env, char *name) // regarde si une variable "name" est une variable local
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

t_localvar	*find_last_localvar(t_localvar *node)
{
	if(!node)
		return;
	while(node->next)
		node = node->next;
	return (node);
}

void	add_localvar(t_localvar **localvar, char *name, char *value) // ajouter nvl variable local
{
	t_localvar	*new_localvar;
	t_localvar	*last;

	new_localvar = malloc(sizeof(t_localvar));
	if(!new_localvar)
		error("Error at malloc in add_localvar");
	new_localvar->name = name;
	new_localvar->value = value; 
	if (!localvar)
	{
		new_localvar->next = NULL;
		new_localvar->prev = NULL;
		*localvar = new_localvar;
	}
	else
	{
		last = find_last_localvar(*localvar);
		last->next = new_localvar;
		new_localvar->prev = last;
	}
}

void	ft_localvar(t_localvar **localvar, t_token *token)
{
	char *name;
	char *value;
	int i;

	while (token->str[i] != '=') // decouper name/ value
		i++;
	name = ft_strndup(token->str, i);
	value = ft_substr(token->str, i + 1);
	add_localvar(localvar, name, value); // creer nvl node contenant la var local de l utilisateur
}