
#include <minishell.h>
#include "exec.h"

void ft_echo(t_token *lst)
{
	int first_word;
	int is_newline;
	t_token *current;

	first_word = 1;
	is_newline = 1;
	current = lst->next;
		
	if (!current)
	{
		printf("\n");
		return;
	}
		
	if (current && current->str && !strcmp(current->str, "-n"))
	{
		is_newline = 0;
		current = current->next;
	}
		
	while (current && current->role == ROLE_ARGUMENT)
	{
		if (!first_word)
			printf(" ");
		
		printf("%s", current->str);
		first_word = 0;
		current = current->next;
	}
	if (is_newline)
		printf("\n");
}
