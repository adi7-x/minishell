#include "minishell.h"

int	syntax_error(t_lexer *lexer)
{
	t_lexer	*current;

	if (!lexer)
		return (1);
	current = lexer;
	// Check for empty input
	if (!current)
		return (1);
	// Check for invalid start
	if (current->token.type != TOKEN_WORD
		&& current->token.type != TOKEN_HEREDOC
		&& current->token.type != TOKEN_INREDIR
		&& current->token.type != TOKEN_OUTREDIR
		&& current->token.type != TOKEN_REDIR_APPEND
		&& current->token.type != TOKEN_PIPE)
		return (1);
	while (current)
	{
		// Check if a pipe is at the start or at the end,
		// or if there are consecutive pipes
		if (current->token.type == TOKEN_PIPE)
		{
			if (current->prev == NULL || current->next == NULL
				|| current->next->token.type == TOKEN_PIPE)
				return (1);
		}
		// Check for valid tokens after redirections
		if ((current->token.type == TOKEN_INREDIR
				|| current->token.type == TOKEN_OUTREDIR
				|| current->token.type == TOKEN_REDIR_APPEND)
			&& (current->next == NULL
				|| current->next->token.type != TOKEN_WORD))
			return (1);
		// Check for invalid tokens between commands
		if (current->prev && current->prev->token.type != TOKEN_PIPE
			&& current->prev->token.type != TOKEN_WORD
			&& current->token.type != TOKEN_PIPE)
			return (1);
		current = current->next;
	}
	// Check if the last token is valid
	if (lexer->token.type == TOKEN_PIPE || (lexer->token.type != TOKEN_WORD
			&& lexer->token.type != TOKEN_HEREDOC
			&& lexer->token.type != TOKEN_REDIR_APPEND
			&& lexer->token.type != TOKEN_INREDIR
			&& lexer->token.type != TOKEN_OUTREDIR))
		return (1);
	return (0);
}

int	check_syntax_errors(t_lexer *lexer)
{
	if (syntax_error(lexer))
	{
		printf("syntax error\n");
		return (1);
	}
	return (0);
}
