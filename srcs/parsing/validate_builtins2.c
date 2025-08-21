#include "minishell.h"

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

static int	check_exit_args(t_token *token, t_shell *shell)
{
	t_token	*current;
	int		arg_count;

	arg_count = 0;
	current = token->next;
	while (current && current->role == ROLE_ARGUMENT)
	{
		arg_count++;
		current = current->next;
	}
	if (arg_count > 1)
	{
		putstr_error("minishell: exit: too many arguments\n");
		shell->exit_code = 1;
		return (0);
	}
	if (arg_count == 1 && !is_numeric(token->next->str))
	{
		putstr_error("minishell: exit: numeric argument required\n");
		shell->exit_code = 2;
		shell->should_exit = 1;
		return (0);
	}
	return (arg_count);
}

int	validate_exit_args(t_token *token, t_shell *shell)
{
	int	arg_count;

	printf("exit\n");
	arg_count = check_exit_args(token, shell);
	if (arg_count == 0)
	{
		shell->should_exit = 1;
		return (1);
	}
	if (arg_count < 0)
		return (1);
	shell->should_exit = 1;
	shell->exit_code = ft_atol(token->next->str) % 256;
	return (1);
}
