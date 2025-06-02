/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprot <cprot@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 09:51:45 by cprot             #+#    #+#             */
/*   Updated: 2025/06/02 11:29:44 by cprot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_node_on_error(t_env *new)
{
	if (new->name)
		free(new->name);
	if (new->value)
		free(new->value);
	free(new);
}

t_env	*create_node(t_env **envp, char *name, char *value)
{
	t_env	*new;
	t_env	*last;

	new = (t_env *)malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->name = ft_strdup(name);
	new->value = ft_strdup(value);
	if (!new->name || !new->value)
	{
		free_node_on_error(new);
		return (NULL);
	}
	new->prev = NULL;
	new->next = NULL;
	if (!(*envp))
		*envp = new;
	else
	{
		last = found_last(*envp);
		last->next = new;
		new->prev = last;
	}
	return (new);
}

void	split_envp(char *envp, char **name, char **value)
{
	int	i;
	int	start;

	i = 0;
	while (envp[i] && envp[i] != '=') // Ajouter && envp[i]
		i++;
	if (!envp[i]) // Pas de '=' trouvé
	{
		*name = NULL;
		*value = NULL;
		return ;
	}
	start = i;
	*name = malloc((i + 1) * sizeof(char));
	if (!(*name))
		return ;
	i = 0;
	while (i < start)
	{
		(*name)[i] = envp[i];
		i++;
	}
	(*name)[i] = '\0';
	*value = ft_substr(envp, start + 1);
	return ;
}

t_env	*init_env_from_envp(char **envp)
{
	char	*name;
	char	*value;
	t_env	*env;
	int		i;

	i = 0;
	env = NULL;
	while (envp[i])
	{
		split_envp(envp[i], &name, &value);
		create_node(&env, name, value);
		free(name);
		free(value);
		i++;
	}
	return (env);
}
