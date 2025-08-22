#include "minishell.h"

int	is_valid_identifier(char *name)
{
	int	i;

	if (!name || !name[0])
		return (0);
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (0);
	i = 0;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	is_numeric(char *str)
{
	int		i;
	long	test;

	i = 0;
	if (str[0] == '+' || str[0] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	test = ft_atol(str);
	if (str[0] == '-' && test > 0) // overflow négatif
		return (0);
	if (str[0] != '-' && test < 0) // overflow positif
		return (0);
	return (1);
}

static int	validate_single_builtin(t_token *current, t_shell *shell)
{
	if (ft_strcmp(current->str, "export") == 0)
	{
		if (!validate_export(current, shell, "export"))
			return (0);
	}
	else if (ft_strcmp(current->str, "unset") == 0)
	{
		if (!validate_unset(current, shell, "unset"))
			return (0);
	}
	else if (ft_strcmp(current->str, "cd") == 0)
	{
		if (!validate_cd_args(current, shell))
			return (0);
	}
	else if (ft_strcmp(current->str, "exit") == 0)
	{
		if (!validate_exit_args(current, shell))
			return (0);
	}
	return (1);
}

int	validate_builtin_arguments(t_token *tokens, t_shell *shell)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (current->role == ROLE_COMMAND)
		{
			if (!validate_single_builtin(current, shell))
				return (0);
		}
		current = current->next;
	}
	return (1);
}
