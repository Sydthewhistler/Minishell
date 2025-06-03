/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprot <cprot@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 10:15:19 by cprot             #+#    #+#             */
/*   Updated: 2025/06/03 11:35:02 by cprot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

char	*get_exit_status_string(void)
{
	extern int	g_exit_status;

	return (ft_itoa(g_exit_status));
}

void	handle_exit_status(int *i, t_token **tokens)
{
	char	*value;

	value = get_exit_status_string();
	(*i)++;
	create_token(tokens, value, CONTENT_WORD);
	free(value);
}

void	handle_variable(char *line, int *i, t_token **tokens, t_env *env)
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
	value = get_env_value(env, name);
	if (!value)
		value = ft_strdup("");
	create_token(tokens, value, CONTENT_WORD);
	free(name);
	if (value)
		free(value);
}
