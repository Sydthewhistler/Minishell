#include "minishell.h"
#include "exec.h"

char *ft_filename(t_token *token)
{
    while(token->role != ROLE_FILENAME)
        token = token->next;
    return (token->str);
}