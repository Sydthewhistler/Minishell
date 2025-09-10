#include "exec.h"
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
	if (str[0] == '-' && test > 0)
		return (0);
	if (str[0] != '-' && test < 0)
		return (0);
	return (1);
}

int	validate_export(t_token *token, t_shell *shell, char *cmd)
{
	t_token	*current;
	char	*name;
	int		i;

	current = token->next;
	while (current && current->role == ROLE_ARGUMENT)
	{
		i = 0;
		while (current->str[i] && current->str[i] != '=')
			i++;
		name = ft_substr_len(current->str, 0, i);
		if (!is_valid_identifier(name))
		{
			putstr_error(cmd);
			putstr_error(": `");
			putstr_error(current->str);
			putstr_error("': not a valid identifier\n");
			shell->exit_code = 1;
			free(name);
			return (0);
		}
		free(name);
		current = current->next;
	}
	return (1);
}

int	validate_unset(t_token *token, t_shell *shell, char *cmd)
{
	t_token	*current;
	char	*name;
	int		i;

	current = token->next;
	(void)cmd;
	while (current && current->role == ROLE_ARGUMENT)
	{
		i = 0;
		while (current->str[i] && current->str[i] != '=')
			i++;
		name = ft_substr_len(current->str, 0, i);
		if (!is_valid_identifier(name))
		{
			shell->exit_code = 0;
			free(name);
			return (0);
		}
		free(name);
		current = current->next;
	}
	return (1);
}

int	validate_cd_args(t_token *token, t_shell *shell)
{
	t_token	*current;
	int		arg_count;

	current = token->next;
	arg_count = 0;
	while (current && current->role == ROLE_ARGUMENT)
	{
		arg_count++;
		current = current->next;
	}
	if (arg_count > 1)
	{
		putstr_error("cd: too many arguments\n");
		shell->exit_code = 1;
		return (0);
	}
	if (arg_count == 1 && access(token->next->str, F_OK) != 0)
	{
		putstr_error("cd: ");
		putstr_error(token->next->str);
		putstr_error(": No such file or directory\n");
		shell->exit_code = 1;
		return (0);
	}
	return (1);
}
