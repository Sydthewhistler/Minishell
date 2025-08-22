/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coraline <coraline@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 10:22:44 by cprot             #+#    #+#             */
/*   Updated: 2025/08/22 10:24:46 by coraline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

char	*create_path(char *dir, char *cmd)
{
	char	*path;
	char	*temp;

	temp = ft_strjoin(dir, "/");
	if (!temp)
		return (NULL);
	path = ft_strjoin(temp, cmd);
	free(temp);
	return (path);
}

static int	is_executable_file(char *full_path)
{
	struct stat	info;

	if (stat(full_path, &info) == 0)
	{
		if (S_ISREG(info.st_mode) && access(full_path, X_OK) == 0)
			return (1);
	}
	return (0);
}

char	*find_in_path(char *cmd, char *path_dirs)
{
	int		i;
	char	**dirs;
	char	*full_path;

	dirs = ft_split(path_dirs, ':');
	if (!dirs)
		return (NULL);
	i = 0;
	while (dirs[i])
	{
		full_path = create_path(dirs[i], cmd);
		if (!full_path)
			return (free_args(dirs), NULL);
		if (is_executable_file(full_path))
			return (free_args(dirs), full_path);
		free(full_path);
		i++;
	}
	free_args(dirs);
	return (NULL);
}

char	*search_path(char *cmd, t_env *env)
{
	t_env	*current;

	current = env;
	if (ft_strrchr(cmd, '/'))
		return (ft_strdup(cmd));
	while (current)
	{
		if (ft_strcmp(current->name, "PATH") == 0)
			return (find_in_path(cmd, current->value));
		current = current->next;
	}
	return (NULL);
}
