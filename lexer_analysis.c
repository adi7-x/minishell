#include "minishell.h"

int	get_quoted_word(t_lexer **lexer, char *input)
{
	int		i;
	char	*word;

	i = 0;
	i = get_next_quote(&input[i]);
	word = malloc(i + 1);
	if (word == NULL)
		return (0);
	strncpy(word, input, i);
	word[i] = '\0';
	add_token(lexer, word, TOKEN_WORD);
	return (i);
}

int	handle_redirection(t_lexer **lexer, char *input)
{
	int		i;
	char	*word;

	i = 0;
	while (input[i] == '>' || input[i] == '<')
		i++;
	word = malloc(i + 1);
	if (word == NULL)
		return (0);
	strncpy(word, input, i);
	word[i] = '\0';
	if (word[0] == '>')
		add_token(lexer, word, TOKEN_OUTREDIR);
	else
		add_token(lexer, word, TOKEN_INREDIR);
	return (i);
}
int	handle_pipe(t_lexer **lexer, char *input)
{
	int		i;
	char	*word;

	i = 0;
	while (input[i] == '|')
		i++;
	word = malloc(i + 1);
	if (word == NULL)
		return (0);
	strncpy(word, input, i);
	word[i] = '\0';
	add_token(lexer, word, TOKEN_PIPE);
	return (i);
}

// this for test:

int	extract_word(t_lexer **lexer, char *input)
{
	int		i;
	int		start;
	int		len;
	char	*word;

	i = 0;
	// Skip over consecutive whitespace characters
	while (isspace(input[i]))
		i++;
	start = i;
	while (!isspace(input[i]) && input[i] != '\0' && input[i] != '>'
		&& input[i] != '<' && input[i] != '|')
		i++;
	len = i - start;
	word = malloc(len + 1);
	if (word == NULL)
		return (0);
	strncpy(word, &input[start], len);
	word[len] = '\0';
	add_token(lexer, word, TOKEN_WORD);
	return (i);
}

// this version works
// int	extract_word(t_lexer **lexer, char *input)
// {
// 	int		i;
// 	char	*word;

// 	i = 0;
// 	while (!isspace(input[i]) && input[i] != '\0' && input[i] != '>'
// 		&& input[i] != '<' && input[i] != '|')
// 		i++;
// 	word = malloc(i + 1);
// 	if (word == NULL)
// 		return (0);
// 	strncpy(word, input, i);
// 	word[i] = '\0';
// 	add_token(lexer, word, TOKEN_WORD);
// 	return (i);
// }

void	ft_lstadd_back(t_lexer **lst, t_lexer *new)
{
	t_lexer	*last;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	last = *lst;
	while (last->next)
		last = last->next;
	last->next = new;
	new->prev = last;
}

// add_token function

void	add_token(t_lexer **lexer, char *value, enum e_token_type type)
{
	t_lexer	*new;
	t_token	*token;

	new = malloc(sizeof(t_lexer));
	if (new == NULL)
		return ;
	new->data = value;
	new->next = NULL;
	new->prev = NULL;
	token = malloc(sizeof(t_token));
	if (token == NULL)
	{
		free(new);
		return ;
	}
	token->value = value;
	token->type = type;
	token->next = NULL;
	new->token = *token;
	ft_lstadd_back(lexer, new);
}
int	get_next_quote(char *input)
{
	int	i;

	i = 1;
	while (input[i] && input[i] != input[0])
		i++;
	if (input[i] == '\0')
		return (0);
	return (i + 1);
}

void	lexer_analysis(char *input, t_lexer **lexer)
{
	int i;

	i = 0;
	*lexer = NULL;
	while (input[i])
	{
		while (isspace(input[i]))
			i++;
		if (input[i] == '\0')
			break ;
		if (input[i] == '"' || input[i] == '\'')
			i += get_quoted_word(lexer, &input[i]);
		else if (input[i] == '>' || input[i] == '<')
			i += handle_redirection(lexer, &input[i]);
		else if (input[i] == '|')
			i += handle_pipe(lexer, &input[i]);
		else
			i += extract_word(lexer, &input[i]);
	}
	check_syntax_errors(*lexer);
}