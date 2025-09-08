/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coraline <coraline@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 14:55:48 by scavalli          #+#    #+#             */
/*   Updated: 2025/08/17 17:09:21 by coraline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "minishell.h"

void	ft_unset(t_localvar **localvar, t_env **env, t_token *token)
{
	t_localvar *del_localvar;
	t_env *del_env;

	if (!token->next)
		return ; // bash ne fait rien si pas d'arguments

	del_localvar = is_local(*localvar, token->next->str);
	if (del_localvar)
	{
		free_localvar(del_localvar, localvar);
		return ;
	}
	del_env = is_env(*env, token->next->str);
	if (del_env)
	{
		free_env(del_env, env);
		return ;
	}
}
