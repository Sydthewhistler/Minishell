/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scavalli <scavalli@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:42:51 by cprot             #+#    #+#             */
/*   Updated: 2025/05/27 17:26:35 by scavalli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	char	*line;
	//t_list	*tokens;
	t_env *env;
	t_localvar *localvar;

	rl_readline_name = "minishell";
	while (1)
	{
		line = readline("minishell>");
		if (!line)
			exit(1);
		if (ft_strcmp(line, "exit") == 0 || ft_strcmp(line, "EXIT") == 0)
		{
			free(line);
			break ;
		}
		if (*line != '\0')
		{
			add_history(line);
			parse_line(line);
			// exec(tokens);
			//free_list(tokens);
		}
		free(line);
	}
	rl_clear_history();
	return (0);
}
