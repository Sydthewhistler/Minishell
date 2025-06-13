/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_master.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scavalli <scavalli@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 12:32:05 by scavalli          #+#    #+#             */
/*   Updated: 2025/06/13 16:32:52 by scavalli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

int	exec_master(t_token *token, t_env **env, t_localvar **localvar)
{
	int	pipe_fd[2];

	pipe(pipe_fd);
	while(1)
	{
		if(!token)
			break;
		while(token && token->role != ROLE_COMMAND) // aller prochaine commande
			token = token->next;
		if(token && is_builtin(token)) //si c est un builtin
			ft_builtin(token, env, localvar, pipe_fd[1]);
		else if(token)
			exec(token, pipe_fd[0], pipe_fd[1]); // si c est une commande autre
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	return (0);
}

