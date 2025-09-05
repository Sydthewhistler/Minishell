#include "minishell.h"

void	skip_whitespace(char *line, int *i, t_shell *shell)
{
	while (line[*i] == ' ' || line[*i] == '\t' || line[*i] == ':')
		(*i)++;
	if (line[*i] == '\\' && line[*i + 1] != '$')
		(*i)++;
	if (line[*i] == '!')
	{
		shell->exit_code = 1;
		(*i)++;
		skip_whitespace(line, i, shell);
	}
}

t_token	*found_last(t_token *tokens)
{
	if (!tokens)
		return (NULL);
	while (tokens->next)
		tokens = tokens->next;
	return (tokens);
}

static t_token	*init_new_token(char *content, int content_type)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->str = ft_strdup(content);
	if (!new_token->str)
	{
		free(new_token);
		return (NULL);
	}
	new_token->type = content_type;
	new_token->role = 0;
	new_token->envp = NULL;
	new_token->next = NULL;
	new_token->prev = NULL;
	new_token->exit_code = -2;
	return (new_token);
}

void	create_token(t_token **tokens, char *content, int content_type)
{
	t_token	*new_token;
	t_token	*last;

	if (!tokens)
		return ;
	new_token = init_new_token(content, content_type);
	if (!new_token)
		return ;
	if (!(*tokens))
		*tokens = new_token;
	else
	{
		last = found_last(*tokens);
		last->next = new_token;
		new_token->prev = last;
	}
}

void	parse_word(char *line, int *i, t_token **tokens, t_shell *shell)
{
	char	*str;
	int		start;

	start = *i;
	while (line[*i] != '\0' && line[*i] != ' ' && line[*i] != '\t'
		&& line[*i] != '>' && line[*i] != '<' && line[*i] != '$'
		&& line[*i] != '\'' && line[*i] != '"')
		(*i)++;
	if (*i > start)
	{
		str = ft_substr_len(line, start, *i - start);
		create_token(tokens, str, CONTENT_WORD);
		free(str);
	}
	skip_whitespace(line, i, shell);
}
