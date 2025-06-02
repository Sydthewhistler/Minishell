/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scavalli <scavalli@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:01:17 by scavalli          #+#    #+#             */
/*   Updated: 2025/06/02 10:06:57 by scavalli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*find_last_exportvar(t_env *node)
{
	if (!node)
		return ;
	while (node->next)
		node = node->next;
	return (node);
}

void	add_exportvar(t_env **env, char *name, char *value)
{
	t_env	*new_exportvar;
	t_env	*last;

	new_exportvar = malloc(sizeof(t_env));
	if (!new_exportvar)
		error("Error at malloc in add_exportvar");
	new_exportvar->name = name;
	new_exportvar->value = value;
	if (!env)
	{
		new_exportvar->next = NULL;
		*env = new_exportvar;
	}
	else
	{
		last = find_last_exportvar(*env);
		last->next = new_exportvar;
	}
}

t_localvar	*is_local(t_localvar *local, char *name) // regarde si une variable "name" est une variable local
{
	if(!local)
		return (NULL);
	while(local)
	{
		if(ft_strcmp(local->name, name) == 0)
			return (local);
		local = local->next;
	}
	return (NULL);
}

void	print_export(t_env *env)
{
	while (env)
	{
		printf("declare -x %s=\"%s\"\n", env->name, env->value); //on print env car enfet export affiche env juste syntaxe qui change ("declare -x")
		env = env->next;
	}
	return ;
}

void	export_local(t_localvar *local,t_env **env, t_localvar **localvar) //ajouter local a env puis supp local
{
	add_exportvar(env, local->name, local->value);
	free_localvar(local);
}

void	ft_export(t_env **env, t_token *token, t_localvar **localvar)
{
	char *name;
	char *value;
	t_localvar *local;
	int i;

	if(!token->next) // si c est juste "export" uniquement afficher les variables exportées
	{
		print_export(*env);
		return ;
	}
	while (token->str[i] != '=') //pour separer name/value
		i++;
	name = ft_strndup(token->str, i); // decoupe name
	local = is_local(*localvar, name);
	if(local) // si c est une variable local on doit la supp de t_env et envoyer vers t_env;
	{
		export_local(local, env, localvar);//ajouter local a env puis supp local
		free(name);
		return ;
	}
	value = ft_substr(token->str, i + 1);
	add_exportvar(env, name, value); // sinon on la cree directement dans la liste export
}
