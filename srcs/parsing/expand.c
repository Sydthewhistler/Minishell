#include "minishell.h"

char	*get_env_value(t_env *env, char *name)
{
	t_env	*cur;

	cur = env;
	while (cur)
	{
		if (ft_strcmp(cur->name, name) == 0)
			return (ft_strdup(cur->value));
		cur = cur->next;
	}
	return (NULL);
}

char	*get_var_value(t_env *env, t_localvar *localvar, char *name)
{
	t_localvar	*local;
	char		*uid_env;

	if (ft_strcmp(name, "UID") == 0)
	{
		uid_env = getenv("UID");
		if (uid_env)
			return (ft_strdup(uid_env));
		else
			return (ft_strdup("1000"));
	}
	local = localvar;
	while (local)
	{
		if (ft_strcmp(local->name, name) == 0)
			return (ft_strdup(local->value));
		local = local->next;
	}
	return (get_env_value(env, name));
}

void	handle_exit_status(int *i, t_token **tokens, t_shell *shell)
{
	char	*value;

	value = ft_itoa(shell->exit_code);
	(*i)++;
	create_token(tokens, value, CONTENT_WORD);
	free(value);
}

void	handle_variable(char *line, int *i, t_token **tokens, t_shell *shell)
{
	char	*name;
	char	*value;
	int		start;

	start = *i;
	while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
		(*i)++;
	if (*i == start)
		return ;
	name = ft_substr_len(line, start, *i - start);
	value = get_var_value(shell->env, *(shell->localvar), name);
	if (!value)
		value = ft_strdup("");
	create_token(tokens, value, CONTENT_WORD);
	free(name);
	if (value)
		free(value);
}

void	parse_var(char *line, int *i, t_token **tokens, t_shell *shell)
{
	(*i)++;
	if (line[*i] == '?')
	{
		handle_exit_status(i, tokens, shell);
		return ;
	}
	if (ft_isdigit(line[*i]))
	{
		(*i)++;
		create_token(tokens, "", CONTENT_WORD);
		return ;
	}
	if (line[*i] == '"' || line[*i] == '\'')
		return ;
	if (!line[*i] || (!ft_isalpha(line[*i]) && line[*i] != '_'))
	{
		create_token(tokens, "$", CONTENT_WORD);
		return ;
	}
	handle_variable(line, i, tokens, shell);
	skip_whitespace(line, i, shell);
}
