/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprot <cprot@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:26:43 by cprot             #+#    #+#             */
/*   Updated: 2025/05/26 14:17:04 by cprot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <unistd.h>

# define COMMAND 1
# define ARGUMENT 2
# define PIPE 3
# define REDIRECT_IN 4     //
# define REDIRECT_OUT 5    // >
# define REDIRECT_APPEND 6 // >>
# define FILENAME 8
# define STRING 9 // Contenu entre guillemets
# define TOKEN_UNKNOWN 0

typedef struct s_list
{
	char			*str;
	int				type;
	struct s_list	*prev;
	struct s_list	*next;
}					t_list;

t_list				*pars_line(char *line);

static void			create_node(t_list **stack, char *token_str,
						int token_type);

#endif