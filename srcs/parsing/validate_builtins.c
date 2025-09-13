/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_builtins.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprot <cprot@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 09:16:09 by cprot             #+#    #+#             */
/*   Updated: 2025/09/13 09:16:10 by cprot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "minishell.h"

static int	count_exit_args(t_token *token)
{
	t_token	*cur;
	int		ac;

	cur = token->next;
	ac = 0;
	while (cur && cur->role == ROLE_ARGUMENT)
	{
		ac++;
		cur = cur->next;
	}
	return (ac);
}

static int	validate_exit_conditions(t_token *token, t_shell *shell, int ac)
{
	if (is_followedpipe(token))
	{
		shell->exit_code = 0;
		return (0);
	}
	if (ac > 1)
	{
		putstr_error("minishell: exit: too many arguments\n");
		shell->exit_code = 1;
		shell->should_exit = 0;
		return (-1);
	}
	if (ac == 1 && !is_numeric(token->next->str))
	{
		putstr_error("minishell: exit: numeric argument required\n");
		shell->exit_code = 2;
		shell->should_exit = 1;
		return (-2);
	}
	return (ac);
}

int	validate_exit_args(t_token *token, t_shell *shell)
{
	int	arg_count;

	arg_count = count_exit_args(token);
	arg_count = validate_exit_conditions(token, shell, arg_count);
	if (arg_count <= 0 && arg_count != -1)
		printf("exit\n");
	if (arg_count == 0)
	{
		shell->should_exit = 1;
		shell->exit_code = 0;
	}
	else if (arg_count == 1)
	{
		printf("exit\n");
		shell->should_exit = 1;
		shell->exit_code = ft_atol(token->next->str) % 256;
	}
	else if (arg_count == -1)
		printf("exit\n");
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
