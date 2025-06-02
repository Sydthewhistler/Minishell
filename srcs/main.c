/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprot <cprot@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:42:51 by cprot             #+#    #+#             */
/*   Updated: 2025/06/02 12:00:40 by cprot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_list(t_token *tokens)
{
	t_token	*cur;

	cur = tokens;
	while (cur)
	{
		printf("token->str : %s \n", cur->str);
		printf("token->type : %d \n", cur->type);
		cur = cur->next;
	}
}

void	print_env(t_env *env)
{
	t_env	*cur;

	cur = env;
	while (cur)
	{
		printf("token->name : %s \n", cur->name);
		printf("token->value : %s \n", cur->value);
		cur = cur->next;
	}
}

void	free_token(t_token **tokens)
{
	t_token	*cur;
	t_token	*tmp;

	if (!tokens)
		return ;
	cur = *tokens;
	while (cur)
	{
		tmp = cur->next;
		if (cur->str)
			free(cur->str);
		free(cur);
		cur = tmp;
	}
	*tokens = NULL;
}

int	main(int ac, char **av, char **envp)
{
	char		*line;
	t_list		*tokens = NULL;
	t_env		*env;
	t_localvar	*localvar;

	(void)ac;
	(void)av;
	env = init_env_from_envp(envp);
	rl_readline_name = "minishell";
	while (1)
	{
		line = readline("minishell>");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (ft_strcmp(line, "exit") == 0)
		{
			free(line);
			break ;
		}
		if (*line != '\0')
		{
			add_history(line);
			parse_line(line, &tokens);
			if (tokens)
			{
				print_list(tokens);
				// exec(tokens);
				free_token(&tokens);
				tokens = NULL;
			}
		}
		free(line);
	}
	rl_clear_history();
	print_env(env);
	return (0);
}
