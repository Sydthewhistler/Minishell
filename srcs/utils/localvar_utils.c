
#include "minishell.h"

void	create_newlocalvar(t_localvar **localvar, char *name, char *value)
{
	t_localvar	*new_node;
	t_localvar	*current;

	new_node = malloc(sizeof(t_localvar));
	if (!new_node)
		return ;
	new_node->name = strdup(name);
	new_node->value = strdup(value);
	new_node->next = NULL;
	new_node->prev = NULL;
	if (!*localvar)
	{
		*localvar = new_node;
		return ;
	}
	current = *localvar;
	while (current->next)
		current = current->next;
	current->next = new_node;
	new_node->prev = current;
}