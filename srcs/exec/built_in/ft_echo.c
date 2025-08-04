
#include "exec.h"
#include <minishell.h>

// void	ft_echo(t_token *lst)
// {
// 	int		first_word;
// 	int		is_newline;
// 	t_token	*current;

// 	first_word = 1;
// 	is_newline = 1;
// 	current = lst->next;
// 	if (!current)
// 	{
// 		printf("\n");
// 		return ;
// 	}
// 	// BOUCLE pour traiter tous les -n consécutifs
// 	while (current && current->str && !strcmp(current->str, "-n"))
// 	{
// 		is_newline = 0;
// 		current = current->next;
// 	}
// 	while (current && current->role == ROLE_ARGUMENT)
// 	{
// 		if (!first_word)
// 			printf(" ");
// 		printf("%s", current->str);
// 		first_word = 0;
// 		current = current->next;
// 	}
// 	if (is_newline)
// 		printf("\n");
// }

static int	should_ignore_n_arg(char *str)
{
	int	i;

	if (str[0] != '-' || str[1] != 'n')
		return (0);
	i = 2;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

static void	print_echo_args(t_token *current, int is_newline)
{
	int	first_word;
	int	skip_n_args;

	first_word = 1;
	skip_n_args = !is_newline;
	while (current && current->role == ROLE_ARGUMENT)
	{
		if (skip_n_args && should_ignore_n_arg(current->str))
			current = current->next;
		else
		{
			skip_n_args = 0;
			if (current->str && current->str[0] != '\0')
			{
				if (!first_word)
					printf(" ");
				printf("%s", current->str);
				first_word = 0;
			}
			current = current->next;
		}
	}
}

void	ft_echo(t_token *lst)
{
	int		is_newline;
	t_token	*current;

	is_newline = 1;
	current = lst->next;
	if (!current)
	{
		printf("\n");
		return ;
	}
	while (current && current->str && !strcmp(current->str, "-n"))
	{
		is_newline = 0;
		current = current->next;
	}
	print_echo_args(current, is_newline);
	if (is_newline)
		printf("\n");
}
