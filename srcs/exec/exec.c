/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprot <cprot@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 09:14:38 by cprot             #+#    #+#             */
/*   Updated: 2025/09/13 09:14:40 by cprot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "minishell.h"

char	**create_cmd(t_token *token)
{
	char	**cmd_arg;
	t_token	*start;
	int		arg_nb;
	int		i;

	arg_nb = 1;
	start = token;
	token = token->next;
	while (token && token->role == ROLE_ARGUMENT)
	{
		arg_nb++;
		token = token->next;
	}
	cmd_arg = malloc(sizeof(char *) * (arg_nb + 1));
	cmd_arg[0] = ft_strdup(start->str);
	start = start->next;
	i = 1;
	while (start && start->role == ROLE_ARGUMENT)
	{
		cmd_arg[i] = ft_strdup(start->str);
		i++;
		start = start->next;
	}
	cmd_arg[i] = NULL;
	return (cmd_arg);
}

void	exec_cmd(t_token *token, char **env_arg, int p_read, int p_write)
{
	pid_t	id;
	char	**cmd_arg;
	int		status;

	cmd_arg = create_cmd(token);
	id = fork();
	if (id < 0)
		error("fork failed");
	if (id == 0)
	{
		redirect(token, p_read, p_write);
		if (execve(token->envp, cmd_arg, env_arg) == -1)
			error("execve failed");
	}
	else
	{
		waitpid(id, &status, 0);
		token->exit_code = WEXITSTATUS(status);
		free_tab(cmd_arg);
	}
}

char	**create_envarg(t_env *env)
{
	char	**envp;
	char	*line;
	int		count;
	t_env	*tmp;

	count = 0;
	tmp = env;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	count = 0;
	while (env)
	{
		line = ft_strjoin(env->name, "=");
		envp[count++] = ft_strjoin(line, env->value);
		free(line);
		env = env->next;
	}
	envp[count] = NULL;
	return (envp);
}

void	exec(t_token *token, t_env *env, int p_read, int p_write)
{
	char	**env_arg;

	env_arg = create_envarg(env);
	exec_cmd(token, env_arg, p_read, p_write);
	free_tab(env_arg);
}
