#include "minishell.h"

void	create_and_advance(t_token **tokens, char *op, int *i, int len)
{
	create_token(tokens, op, CONTENT_OPERATOR);
	(*i) += len;
}