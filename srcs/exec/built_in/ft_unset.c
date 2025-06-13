/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scavalli <scavalli@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 14:55:48 by scavalli          #+#    #+#             */
/*   Updated: 2025/06/13 17:05:04 by scavalli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

void	ft_unset(t_localvar **localvar, t_env **env, t_token *token)
{
	t_localvar *del_localvar;
	t_env *del_env;

	del_localvar = is_local(*localvar, token->next->str); //cherche si la variable a supprimer est local
	if(del_localvar) // si oui la supp de localvar
	{
		free_localvar(del_localvar, localvar);
		return ;
	}
	del_env = is_env(*env, token->next->str); // cherche si la variable a supprimer est exporte
	if(del_env) //si oui supp de t_env
	{
		free_env(del_env, env);
		return ;
	}
}
