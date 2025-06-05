/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scavalli <scavalli@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:06:12 by scavalli          #+#    #+#             */
/*   Updated: 2025/06/05 21:41:33 by scavalli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H
# include "minishell.h"
# include <stdbool.h>

/*                MASTER FUNCTIONS              */

int			exec_master(t_token *token, t_env **env, t_localvar **localvar);

/*             GENERAL UTILS FUNCTIONS                */

void		error(char *str);
void	putstr_error(char *str);
char		*ft_strndup(char *str, int n);
void	which_built_in(t_token *token, t_env **env, t_localvar **localvar);
bool	is_builtin(t_token *token);

/*                BUILT_IN FUNCTIONS                */

void		ft_echo(char *cmd, t_token *lst);
void	ft_pwd(t_env *env);
void	ft_cd(t_token *token, t_env *env);
void		ft_env(t_env *env);
void		unset(t_localvar **localvar, t_env **env, t_token *token);
void		ft_export(t_env **env, t_token *token, t_localvar **localvar);
void	ft_localvar(t_localvar **localvar, t_token *token); // variables local
int	ft_builtin(t_token *token, t_env **env, t_localvar **localvar, int pipe);

/*               BUILT_IN UTILS FUNCTIONS            */

t_localvar	*is_local(t_localvar *local, char *name);
t_localvar	*is_env(t_env *env, char *name);
void		free_localvar(t_localvar *localvar);
t_env	*find_last_exportvar(t_env *node);
char *return_env_value(char *name, t_env *env);
t_env	*find_env_var(t_env *env, const char *name);

/*                   EXEC FUNCTIONS                  */

void		exec(t_token *token, int p_read, int p_write);

/*                   EXEC UTILS FUNCTIONS             */

bool	is_redirectin(t_token *token);
bool	is_redirectout(t_token *token);
bool	is_precededpipe(t_token *token);
bool	is_followedpipe(t_token *token);



#endif