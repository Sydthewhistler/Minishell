
#include "minishell.h"

void	replace_pwd_env(t_env *env)
{

}

void	ft_cd(t_token *token, t_env *env)
{
	char *path;

	if (token->next && token->next->str && !ft_strcmp(token->next->str, "~")) // obtenir path sauf si path=~ car "cd ~" equivaut a "cd" traite en dessous
		path = token->next->str;
	if(!ft_strcmp(path, ".")) // si "cd ." (path = .)ne rien faire
		return ;
	else
		path = return_env_value("HOME", env); // si "cd" sans path specifie renvoyer au home
	if (!path)
	{
		putstr_error("cd: HOME not set\n");
		return;
	}
	if (chdir(path) != 0)
	{
		putstr_error("cd: no such file or directory: \0");
		putstr_error(path);
	}
	// remplacer PWD dans env
}

static char	*go_back(char *pwd)
{
	char	*last_slash;

	if (!pwd || !ft_strcmp(pwd, "/"))
		return (ft_strdup("/"));
	last_slash = ft_strrchr(pwd, '/');
	if (!last_slash || last_slash == pwd)
		return (ft_strdup("/"));
	return (ft_strndup(pwd, last_slash - pwd));
}

static char	*join_path(char *base, char *add)
{
	char	*result;

	if (!ft_strcmp(base, "/"))
		result = ft_strjoin("/", add);
	else
	{
		result = ft_strjoin(base, "/");
		result = ft_strjoin_free(result, add);
	}
	return (result);
}

int	update_pwd(t_env *env, const char *new_path)
{
	t_env	*pwd;
	t_env	*oldpwd;
	char	*current;
	char	*new_pwd;
	char	**split;
	int		i;

	if (!env || !new_path)
		return (-1);
	pwd = find_env_var(env, "PWD");
	if (!pwd)
		return (-1);
	current = pwd->value ? pwd->value : "/";
	new_pwd = (new_path[0] == '/') ? ft_strdup("/") : ft_strdup(current);
	split = ft_split(new_path, '/');
	i = (new_path[0] == '/') ? 0 : 0;
	while (split && split[i])
	{
		if (!ft_strcmp(split[i], ".."))
			new_pwd = ft_strdup_free(go_back(new_pwd));
		else if (ft_strcmp(split[i], ".") && ft_strlen(split[i]) > 0)
			new_pwd = ft_strdup_free(join_path(new_pwd, split[i]));
		i++;
	}
	oldpwd = find_env_var(env, "OLDPWD");
	if (oldpwd)
		oldpwd->value = ft_strdup_free(ft_strdup(current));
	pwd->value = ft_strdup_free(new_pwd);
	ft_free_split(split);
	return (0);
}