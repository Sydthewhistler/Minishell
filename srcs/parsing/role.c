#include "minishell.h"

void	assign_operator_roles(t_token **tokens)
{
	t_token	*current;

	current = *tokens;
	while (current)
	{
		if (current->type == CONTENT_OPERATOR)
		{
			if (ft_strcmp(current->str, "|") == 0)
				current->role = ROLE_PIPE;
			else if (ft_strcmp(current->str, "<<") == 0)
				current->role = ROLE_REDIRECT_HEREDOC;
			else if (ft_strcmp(current->str, ">>") == 0)
				current->role = ROLE_REDIRECT_APPEND;
			else if (ft_strcmp(current->str, "<") == 0)
				current->role = ROLE_REDIRECT_IN;
			else if (ft_strcmp(current->str, ">") == 0)
				current->role = ROLE_REDIRECT_OUT;
		}
		current = current->next;
	}
}

static int	handle_argument_state(t_token *current, t_parser_state *state)
{
	if (current->type == CONTENT_WORD || current->type == CONTENT_QUOTED)
		current->role = ROLE_ARGUMENT;
	else if (current->type == CONTENT_OPERATOR)
	{
		if (current->role == ROLE_PIPE)
			*state = EXP_CMD;
		else if (current->role == ROLE_REDIRECT_OUT
			|| current->role == ROLE_REDIRECT_APPEND
			|| current->role == ROLE_REDIRECT_IN)
			*state = EXP_FILE;
		else if (current->role == ROLE_REDIRECT_HEREDOC)
			*state = EXP_ARG;
	}
	return (1);
}

static int	handle_filename_state(t_token *current, t_parser_state *state,
		t_shell *shell)
{
	if (!current || !current->str)
		return (0);
	if (current->type == CONTENT_WORD || current->type == CONTENT_QUOTED)
	{
		current->role = ROLE_FILENAME;
		*state = EXP_ARG;
	}
	else if (current->type == CONTENT_HEREDOC)
	{
		current->role = ROLE_HEREDOC_CONTENT;
		*state = EXP_ARG;
	}
	else if (current->type == CONTENT_OPERATOR)
	{
		ft_error_syntax(current->str, shell);
		return (0);
	}
	return (1);
}

static int	process_current_token(t_token *current, t_parser_state *state,
		t_shell *shell)
{
	if (*state == EXP_CMD)
	{
		if (!handle_command_state(current, state, shell))
			return (-1);
	}
	else if (*state == EXP_ARG)
	{
		if (!handle_argument_state(current, state))
			return (-2);
	}
	else if (*state == EXP_FILE)
	{
		if (!handle_filename_state(current, state, shell))
			return (-3);
	}
	return (0);
}

int	apply_role(t_token **tokens, t_shell *shell)
{
	t_parser_state	state;
	t_token			*current;
	int				result;

	current = *tokens;
	state = EXP_CMD;
	assign_operator_roles(tokens);
	while (current)
	{
		result = process_current_token(current, &state, shell);
		if (result != 0)
			return (result);
		current = current->next;
	}
	if (state == EXP_FILE)
	{
		ft_error_syntax("newline", shell);
		return (-3);
	}
	return (0);
}
