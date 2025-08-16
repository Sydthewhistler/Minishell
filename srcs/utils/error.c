
#include "exec.h"
#include <minishell.h>

void	putstr_error(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		write(2, &str[i], 1);
		i++;
	}
}

void	ft_error_cmd_not_found(char *cmd, t_shell *shell)
{
	printf("%s: command not found\n", cmd);
	shell->exit_code = COMMAND_NOT_FOUND;
}

void	ft_error_permission_denied(char *cmd, t_shell *shell)
{
	printf("minishell: %s: Permission denied\n", cmd);
	shell->exit_code = PERMISSION_DENIED;
}

void	ft_error_file(char *cmd, int is_dir, t_shell *shell)
{
	if (is_dir)
	{
		printf("minishell: %s: Is a directory\n", cmd);
		shell->exit_code = PERMISSION_DENIED;
	}
	else
	{
		printf("minishell: %s: No such file or directory\n", cmd);
		shell->exit_code = COMMAND_NOT_FOUND;
	}
}

void	ft_error_syntax(char *token, t_shell *shell)
{
	if (token)
		printf("minishell: syntax error near unexpected token '%s'\n", token);
	else
		printf("minishell: syntax error near unexpected token 'newline'\n");
	shell->exit_code = SYNTAX_ERROR_CODE;
}
