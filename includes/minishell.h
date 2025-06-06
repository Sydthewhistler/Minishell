#ifndef MINISHELL_H
# define MINISHELL_H

//# include "exec.h"
# include "libft.h"
# include <dirent.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

// CONTENT types (ce que contient le token)
# define CONTENT_WORD 1
# define CONTENT_QUOTED 2
# define CONTENT_OPERATOR 3
# define CONTENT_HEREDOC 4 // ← Ajout pour le contenu des heredocs

// ROLE types (fonction du token dans la commande)
# define ROLE_COMMAND 1
# define ROLE_ARGUMENT 2
# define ROLE_FILENAME 3
# define ROLE_PIPE 4
# define ROLE_REDIRECT_IN 5     //
# define ROLE_REDIRECT_OUT 6    // >
# define ROLE_REDIRECT_APPEND 7 // ← Ajout pour >>
# define ROLE_REDIRECT_HEREDOC 8
# define ROLE_HEREDOC_CONTENT 9

typedef enum
{
	EXP_CMD,
	EXP_ARG,
	EXP_FILE
}					t_parser_state;

typedef struct s_token
{
	char			*str;
	int				type;
	int				role;
	char			*envp; // ex /bin/sh/ls
	struct s_token	*next;
	struct s_token	*prev;
}					t_token;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
	struct s_env	*prev;
}					t_env;

// typedef struct s_localvar
// {
// 	char				*name;
// 	char				*value;
// 	struct s_localvar	*next;
// 	struct s_localvar	*prev;
// }						t_localvar;

// UTILS
void				create_token(t_token **tokens, char *content,
						int content_type);
void				skip_whitespace(char *line, int *i);
void				parse_word(char *line, int *i, t_token **tokens);
void				parse_line(char *line, t_token **tokens, t_env *env);

// HEREDOC
char				*extract_delimiter(char *s);
char				*handle_heredoc(char *delimiter);
void				update_history_entry(char *line, char *content,
						char *delimiter);

// ENV
t_env				*init_env_from_envp(char **envp);
char				*get_env_value(t_env *env, char *name);

// EXPAND
void				handle_exit_status(int *i, t_token **tokens);
void				handle_variable(char *line, int *i, t_token **tokens,
						t_env *env);
void				parse_var(char *line, int *i, t_token **tokens, t_env *env);
char				*handle_expand_in_quotes(char *str, t_env *env);
char				*get_exit_status_string(void);

// ROLE
void				apply_role(t_token **tokens, t_env *env);
int					handle_command_state(t_token *current,
						t_parser_state *state, t_env *env);

#endif