
#include "minishell.h"

bool	is_redirectin(t_token *token)
{
	token = token->prev;
	while(token)
	{
		if(token->role == ROLE_COMMAND)
			break;
		if(token->role == ROLE_REDIRECT_IN)
			return(true);
		token = token->prev;
	}
	return (false);
}
bool	is_redirectout(t_token *token)
{
	token = token->next;
	while(token)
	{
		if(token->role == ROLE_COMMAND)
			break;
		if(token->role == ROLE_REDIRECT_OUT)
			return(true);
		token = token->next;
	}
	return (false);
}

bool	is_precededpipe(t_token *token)
{
	token = token->prev;
	while(token)
	{
		if(token->role == ROLE_COMMAND)
			break;
		if(token->role == ROLE_PIPE)
			return(true);
		token = token->prev;
	}
	return (false);
}
bool	is_followedpipe(t_token *token)
{
	token = token->next;
	while(token)
	{
		if(token->role == ROLE_COMMAND)
			break;
		if(token->role == ROLE_PIPE)
			return(true);
		token = token->next;
	}
	return (false);
}

char *find_rdin_file(t_token *token)
{
	token = token->prev;
	while(token)
	{
		if(token->role == ROLE_FILENAME)
			return(token->str);
		token = token->prev;
	}
	return (NULL);
}

char *find_rdout_file(t_token *token)
{
	token = token->next;
	while(token)
	{
		if(token->role == ROLE_FILENAME)
			return(token->str);
		token = token->next;
	}
	return (NULL);
}