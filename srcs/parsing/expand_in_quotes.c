/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_in_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprot <cprot@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 10:51:28 by cprot             #+#    #+#             */
/*   Updated: 2025/06/04 13:01:46 by cprot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*add_char_and_free(char *str, char c)
{
	char	*new;
	int		i;

	i = 0;
	new = malloc(ft_strlen(str) + 2);
	if (!new)
	{
		free(str);
		return (NULL);
	}
	while (str[i])
	{
		new[i] = str[i];
		i++;
	}
	new[i] = c;
	new[i + 1] = '\0';
	free(str);
	return (new);
}

char	*handle_exit_status_in_quotes(char *result, int *i)
{
	char *value;
	char *new_result;

	(*i)++; // Skip le '?'
	value = get_exit_status_string();
	new_result = ft_strjoin(result, value);
	free(result);
	free(value);
	return (new_result);
}

char	*handle_variable_in_quotes(char *result, char *str, int *i, t_env *env)
{
	int		start;
	char	*name;
	char	*value;
	char	*new_result;

	start = *i;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	if (*i == start)
		return (result);
	name = ft_substr_len(str, start, *i - start);
	if (!name)
	{
		free(result);
		return (NULL);
	}
	value = get_env_value(env, name);
	if (!value)
		value = ft_strdup("");
	new_result = ft_strjoin(result, value);
	free(result);
	free(name);
	free(value);
	return (new_result);
}

char	*expand_variable_simple(char *str, int *i, t_env *env, char *result)
{
	(*i)++; // Skip $
	if (str[*i] == '?')
		result = handle_exit_status_in_quotes(result, i);
	else
		result = handle_variable_in_quotes(result, str, i, env);
	return (result);
}

char	*handle_expand_in_quotes(char *str, t_env *env)
{
	char	*result;
	int		i;

	result = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] && (str[i + 1] == '?'
				|| ft_isalpha(str[i + 1]) || str[i + 1] == '_'))
			result = expand_variable_simple(str, &i, env, result);
		else if (str[i] == '$' && str[i + 1] && ft_isdigit(str[i + 1]))
		{
			i++; // Skip $
			while (str[i] && ft_isdigit(str[i]))
				i++; // Skip digits  et apres rentre dans la boucle else
		}
		else
		{
			result = add_char_and_free(result, str[i]);
			i++;
		}
	}
	return (result);
}


// A GERER : $$ et ${}