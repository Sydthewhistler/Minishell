#include "minishell.h"

void	parse_heredoc(char *s, int *i, t_token **tokens)
{
	char	*delimiter;

	*i += 2;
	while (s[*i] == ' ' || s[*i] == '\t')
		(*i)++;
	if (!s[*i] || s[*i] == '\n' || s[*i] == '!' || s[*i] == '<' || s[*i] == '>')
		return (create_token(tokens, "<<", CONTENT_OPERATOR));
	delimiter = extract_delimiter(s + *i);
	if (!delimiter || !delimiter[0])
	{
		if (delimiter)
			free(delimiter);
		return (create_token(tokens, "<<", CONTENT_OPERATOR));
	}
	process_heredoc_content(delimiter, tokens, s, i);
	free(delimiter);
}

void	parse_operator(char *line, int *i, t_token **tokens, t_shell *shell)
{
	char	s[4];

	if (line[*i] == '<' && line[*i + 1] == '<' && line[*i + 2] == '<')
		create_and_advance(tokens, "<<<", i, 3);
	else if (line[*i] == '>' && line[*i + 1] == '>')
		create_and_advance(tokens, ">>", i, 2);
	else if (line[*i] == '<' && line[*i + 1] == '<')
	{
		parse_heredoc(line, i, tokens);
		return ;
	}
	else if (line[*i] == '<' && line[*i + 1] == '>')
		create_and_advance(tokens, "<>", i, 2);
	else if (line[*i] == '|' && line[*i + 1] == '|')
		create_and_advance(tokens, "||", i, 2);
	else if (line[*i] == '&' && line[*i + 1] == '&')
		create_and_advance(tokens, "&&", i, 2);
	else
	{
		s[0] = line[*i];
		s[1] = '\0';
		create_and_advance(tokens, s, i, 1);
	}
	skip_whitespace(line, i, shell);
}

void	handle_segment_parsing(char *line, int *i, t_token **tokens,
		t_shell *shell)
{
	char	*segment;

	segment = parse_complete_segment(line, i, shell);
	if (segment && segment[0])
	{
		create_token(tokens, segment, CONTENT_WORD);
		free(segment);
	}
}

void	parse_line_character(char *line, int *i, t_token **tokens,
		t_shell *shell)
{
	if (line[*i] == '<' && line[*i + 1] == '<')
		parse_heredoc(line, i, tokens);
	else if (line[*i] == '|' || line[*i] == '<' || line[*i] == '>'
		|| line[*i] == '&')
		parse_operator(line, i, tokens, shell);
	else
		handle_segment_parsing(line, i, tokens, shell);
}

int	parse_line(char *line, t_token **tokens, t_shell *shell)
{
	int	i;

	i = 0;
	skip_whitespace(line, &i, shell);
	while (line[i])
	{
		parse_line_character(line, &i, tokens, shell);
		skip_whitespace(line, &i, shell);
	}
	if (apply_role(tokens, shell) != 0)
		return (0);
	if (!validate_builtin_arguments(*tokens, shell))
		return (0);
	return (1);
}
