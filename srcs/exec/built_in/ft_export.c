/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coraline <coraline@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:01:17 by scavalli          #+#    #+#             */
/*   Updated: 2025/08/17 16:42:55 by coraline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

void	add_exportvar(t_env **env, char *name, char *value)
{
	t_env	*new_exportvar;
	t_env	*last;

	new_exportvar = malloc(sizeof(t_env));
	if (!new_exportvar)
		error("Error at malloc in add_exportvar");
	new_exportvar->name = ft_strdup(name);
	new_exportvar->value = ft_strdup(value);
	if (!*env)
	{
		new_exportvar->next = NULL;
		new_exportvar->prev = NULL;
		*env = new_exportvar;
	}
	else
	{
		last = find_last_exportvar(*env);
		last->next = new_exportvar;
		new_exportvar->prev = last;
		new_exportvar->next = NULL;
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

void	ft_export(t_env **env, t_token *token, t_localvar **localvar)
{
	char *name;
	char *value;
	t_localvar *local;
	t_env *is_inenv;
	int i;

	if(!token->next) // si c est juste "export" uniquement afficher les variables exportées
	{
		print_export(*env);
		return ;
	}
	i = 0;
	while (token->next->str[i] && token->next->str[i] != '=') //pour separer name/value
		i++;
	name = ft_strndup(token->next->str, i); // decoupe name
	local = is_local(*localvar, name);
	if (!ft_contains(token->next->str , "=")) //si juste nom VAR "export VAR" aller chercher nom dans localvar
	{
		is_inenv = is_env(*env, token->next->str); // si est dans env, on la modifie donc supp tout pour reconstruire
		if(is_inenv)
			free_env(is_inenv, env);
		if(local) // si VAR existe dans localvar
		{
			add_exportvar(env, local->name, local->value);
			free_localvar(local, localvar);
		}
		free(name);
		return ; //sinon aucune info on return
	}
	is_inenv = is_env(*env, name);
	if(is_inenv)
		free_env(is_inenv,env);
	value = ft_substr(token->next->str, i + 1);
	add_exportvar(env, name, value);
	if(local) // si c est une variable local on doit la supp de t_localvar
		free_localvar(local, localvar);
}
