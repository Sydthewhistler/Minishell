/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scavalli <scavalli@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 14:55:48 by scavalli          #+#    #+#             */
/*   Updated: 2025/05/28 15:30:02 by scavalli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	unset(t_localvar **localvar, t_env **env, t_token *token)
{
	t_localvar *del_localvar;
	t_env *del_env;

	localvar = is_local(*localvar, token->next->str);
	if(localvar)
	{
		if(del_localvar->next)
			*localvar = del_localvar->next;
		else
			*localvar = NULL;
		free_localvar(localvar);
		return ;
	}
	del_env = is_env(*env, token->next->str);
	if(env)
	{
		if(del_env->next)
			*env = del_env->next;
		else
			*env = NULL;
		free_env(env);
		return ;
	}
}
