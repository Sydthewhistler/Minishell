
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
	putstr_error(cmd);
	putstr_error(": command not found\n");
	// Pas de%s possible avec putstr_error
	shell->exit_code = COMMAND_NOT_FOUND;
}

void	ft_error_permission_denied(char *cmd, t_shell *shell)
{
	putstr_error("minishell: ");
	putstr_error(cmd);
	putstr_error(": Permission denied\n");
	shell->exit_code = PERMISSION_DENIED;
}

void	ft_error_file(char *cmd, int is_dir, t_shell *shell)
{
	if (is_dir)
	{
		putstr_error("minishell: ");
		putstr_error(cmd);
		putstr_error(": Is a directory\n");
		shell->exit_code = PERMISSION_DENIED;
	}
	else
	{
		putstr_error("minishell: ");
		putstr_error(cmd);
		putstr_error(": No such file or directory\n");
		shell->exit_code = COMMAND_NOT_FOUND;
	}
}

void	ft_error_syntax(char *token, t_shell *shell)
{
	if (token)
	{
		putstr_error("minishell: syntax error near unexpected token '");
		putstr_error(token);
		putstr_error("'\n");
	}
	else
		putstr_error("minishell: syntax error near unexpected token 'newline'\n");
	shell->exit_code = SYNTAX_ERROR_CODE;
}
