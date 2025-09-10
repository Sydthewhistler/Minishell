#include "exec.h"
#include "minishell.h"

char	**parse_path_parts(char *path)
{
	char	**parts;
	char	**p;
	char	*token;

	parts = malloc(sizeof(char *) * 1000);
	p = parts;
	token = ft_strtok(path, '/');
	while (token)
	{
		if (ft_strcmp(token, "..") == 0 && p > parts)
			p--;
		else if (ft_strcmp(token, ".") != 0 && ft_strlen(token) > 0)
			*p++ = token;
		token = ft_strtok(NULL, '/');
	}
	*p = NULL;
	return (parts);
}

char	*build_path_from_parts(char **parts)
{
	char	*result;
	char	**ptr;

	result = malloc(1000);
	ft_strcpy(result, "/");
	ptr = parts;
	while (*ptr)
	{
		if (ptr != parts)
			ft_strcat(result, "/");
		ft_strcat(result, *ptr);
		ptr++;
	}
	return (result);
}

char	*normalize_path(char *path)
{
	char	**parts;
	char	*result;

	parts = parse_path_parts(path);
	result = build_path_from_parts(parts);
	free(parts);
	return (result);
}

char	*resolve_relative_path(char *current_pwd, char *relative_path)
{
	char	*temp_path;
	char	*resolved;
	int		len;

	len = ft_strlen(current_pwd) + ft_strlen(relative_path) + 2;
	temp_path = malloc(len);
	if (!temp_path)
		return (NULL);
	ft_strcpy(temp_path, current_pwd);
	if (current_pwd[ft_strlen(current_pwd) - 1] != '/')
		ft_strcat(temp_path, "/");
	ft_strcat(temp_path, relative_path);
	resolved = normalize_path(temp_path);
	free(temp_path);
	return (resolved);
}

void	update_pwd(t_env *env, char *new_path)
{
	char	*current_pwd;
	char	*new_pwd;

	set_env_value(env, "OLDPWD", return_env_value("PWD", env));
	if (new_path[0] == '/')
		new_pwd = ft_strdup(new_path);
	else if (!strcmp(return_env_value("PWD", env), "/") && !strcmp(new_path,
			".."))
		return ;
	else
	{
		current_pwd = return_env_value("PWD", env);
		if (!current_pwd)
			return ;
		new_pwd = resolve_relative_path(current_pwd, new_path);
	}
	if (new_pwd)
	{
		set_env_value(env, "PWD", new_pwd);
		free(new_pwd);
	}
}
