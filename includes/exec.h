/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scavalli <scavalli@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:06:12 by scavalli          #+#    #+#             */
/*   Updated: 2025/05/27 18:17:23 by scavalli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
#define EXEC_H

void	error(char *str);
char	*ft_strndup(char *str, int n);
void	free_localvar(t_localvar *localvar);

void	ft_echo(char *cmd, t_token *lst);
void	ft_pwd(void);
void	ft_cd(char *line);











#endif