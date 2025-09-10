#include "exec.h"
#include "minishell.h"

bool	is_redirectout(t_token *token)
{
	token = token->next;
	while (token)
	{
		if (token->role == ROLE_COMMAND)
			break ;
		if (token->role == ROLE_REDIRECT_OUT)
			return (true);
		token = token->next;
	}
	return (false);
}

bool	is_append(t_token *token)
{
	t_token	*current;

	current = token->next;
	while (current)
	{
		if (current->role == 7)
			return (true);
		if (current->role == ROLE_COMMAND)
			return (false);
		current = current->next;
	}
	return (false);
}

void	check_if_redirect_next(t_token *token, int *fd, int p_write)
{
	if (is_redirectout(token))
	{
		*fd = open(find_rdout_file(token), O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (*fd != -1)
			dup2(*fd, STDOUT_FILENO);
	}
	else if (is_append(token))
	{
		*fd = open(ft_filename(token), O_WRONLY | O_CREAT | O_APPEND, 0644);
		dup2(*fd, STDOUT_FILENO);
	}
	else if (is_followedpipe(token))
		dup2(p_write, STDOUT_FILENO);
}

void	check_if_preceded_redirect(t_token *token, int *fd, int p_read)
{
	int	heredoc_handled;

	heredoc_handled = handle_heredoc_redirect(token);
	if (!heredoc_handled && is_redirectin(token))
	{
		*fd = open(find_rdin_file(token), O_RDONLY);
		if (*fd != -1)
			dup2(*fd, STDIN_FILENO);
	}
	if (!heredoc_handled && is_precededpipe(token))
		dup2(p_read, STDIN_FILENO);
}

void	redirect(t_token *token, int p_read, int p_write)
{
	int	fd;

	fd = -1;
	check_if_preceded_redirect(token, &fd, p_read);
	check_if_redirect_next(token, &fd, p_write);
	if (fd != -1)
		close(fd);
	if (p_read != -1)
		close(p_read);
	if (p_write != -1)
		close(p_write);
}
