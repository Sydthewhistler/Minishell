/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scavalli <scavalli@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:42:51 by cprot             #+#    #+#             */
/*   Updated: 2025/05/26 16:15:01 by scavalli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	char	*line;
	t_list	*tokens;

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
			tokens = pars_line(line);
			exec_master(tokens);
			free_list(tokens);
		}
		free(line);
	}
	rl_clear_history();
	return (0);
}
