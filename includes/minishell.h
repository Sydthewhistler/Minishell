/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprot <cprot@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:26:43 by cprot             #+#    #+#             */
/*   Updated: 2025/05/22 14:43:39 by cprot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdio.h>
# include <unistd.h>

# define CMD1 1
# define CMD2 2
//...

typedef struct s_list
{
	char			*str;
	int				type;
	struct s_list	*prev;
	struct s_list	*next;
}					t_list;

t_list	*pars_line(char *line);

static void	create_node(t_list **stack, char *token_str, int token_type);

#endif