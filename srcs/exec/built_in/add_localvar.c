
#include "minishell.h"
#include "exec.h"

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

void	ft_localvar(t_localvar **localvar, t_token *token)
{
	char		*name;
	char		*value;
	int			i;
	t_localvar	*is_in;

	i = 0;
	while (token->str[i] != '=')
		i++;
	name = ft_strndup(token->str, i);
	value = ft_substr(token->str, i + 1);
	is_in = is_local(*localvar, name);
	if(is_in)
		free_localvar(is_in, localvar);
	create_newlocalvar(localvar, name, value);
}
