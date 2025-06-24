/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_master.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scavalli <scavalli@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 12:32:05 by scavalli          #+#    #+#             */
/*   Updated: 2025/06/24 10:43:37 by scavalli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"
#include "time.h"

int	exec_master(t_token *token, t_env **env, t_localvar **localvar)
{
	int	pipe_fd[2];
	int prev_pipe;

	prev_pipe = -1;
	while(1)
	{
		pipe(pipe_fd);
		if(!token)
			break;
		while(token && token->role != ROLE_COMMAND) // aller prochaine commande
			token = token->next;
		if(!token)
			break;
		if(token && is_builtin(token))//si c est un builtin
		{
			ft_builtin(token, env, localvar, pipe_fd[1]);
		}
		else if(token)
			exec(token, *env, prev_pipe, pipe_fd[1]); // si c est une commande autre
		if (prev_pipe != -1)
			close(prev_pipe);
		close(pipe_fd[1]);
		prev_pipe = pipe_fd[0];
		token = token->next;
	}
	if (prev_pipe != -1)
		close(prev_pipe);
	return (0);
}

