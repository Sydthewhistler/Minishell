/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_master.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprot <cprot@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 12:32:05 by scavalli          #+#    #+#             */
/*   Updated: 2025/08/30 14:45:21 by cprot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "minishell.h"
#include "time.h"

int	exec_master(t_token *token, t_env **env, t_localvar **localvar)
{
	int	pipe_fd[2];
	int	prev_pipe;

	prev_pipe = -1;
	ft_setup_execution_signal();
	while(1)
	{
		pipe(pipe_fd);
		if (!token)
			break ;
		while (token && token->role != ROLE_COMMAND) // aller prochaine commande
			token = token->next;
		if (!token)
			break ;
		if (token && is_builtin(token)) // si c est un builtin
		{
			ft_builtin(token, env, localvar, pipe_fd[1]);
		}
		else if (token)
			exec(token, *env, prev_pipe, pipe_fd[1]);
		// si c est une commande autre
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

int	handle_heredoc_redirect(t_token *token)
{
	t_token	*current;
	t_token	*content_token;
	int		heredoc_pipe[2];

	current = token;
	while (current)
	{
		if (current->role == 8 && current->str && ft_strcmp(current->str,
				"<<") == 0)
		{
			content_token = current->next;
			if (content_token && content_token->type == 4)
			{
				if (pipe(heredoc_pipe) == -1)
				{
					perror("heredoc pipe failed");
					exit(1);
				}
				write(heredoc_pipe[1], content_token->str,
					ft_strlen(content_token->str));
				close(heredoc_pipe[1]);
				if (dup2(heredoc_pipe[0], STDIN_FILENO) == -1)
				{
					perror("dup2 failed");
					exit(1);
				}
				close(heredoc_pipe[0]);
				return (1);
			}
		}
		current = current->next;
	}
	return (0);
}

