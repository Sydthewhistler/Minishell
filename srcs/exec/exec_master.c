/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_master.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scavalli <scavalli@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 12:32:05 by scavalli          #+#    #+#             */
/*   Updated: 2025/06/02 15:01:53 by scavalli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_master(t_token *token, t_env **env, t_localvar **localvar)
{
	int i;
	int	pipe[2];

	while(1)
	{
		while(token->role != ROLE_COMMAND) // aller prochaine commande
			token = token->next;
		if(is_builtin(token->str)) //si c est un builtin
			ft_builtin(token, env, localvar, pipe[1]);
		else
		{
			
		}
	}
	
}


/*

Pour built in :

	token Apres : check si redirect out, pipe

*/