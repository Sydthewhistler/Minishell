#include "exec.h"
#include "minishell.h"

bool is_append(t_token *token)
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
