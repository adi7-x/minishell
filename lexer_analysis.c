#include "minishell.h"
#include <limits.h>

// // ft_strncpy function:

// void	ft_strncpy(char *dst, const char *src, size_t len)
// {
//     if (!dst || !src)
//         return ;
//     size_t	i;

//     i = 0;
//     while (src[i] && i < len)
//     {
//         dst[i] = src[i];
//         i++;
//     }
//     dst[i] = '\0';
// }


// void add_token_to_lexer(t_lexer **lexer, t_lexer *new_token)
// {
//     t_lexer *current;

//     if (*lexer == NULL)
//     {
//         *lexer = new_token; 
//     }
//     else
//     {
//         current = *lexer;
//         while (current->next)  
//             current = current->next;
//         current->next = new_token;  
//         new_token->prev = current;
//     }
// }


// int handle_redirection(t_lexer **lexer, char *input)
// {
//     t_lexer *new_token;
//     int count = 0;
//     int token_type;

//     // Determine the type of redirection and count consecutive characters
//     if (input[0] == '<')
//         token_type = TOKEN_INREDIR;
//     else if (input[0] == '>')
//         token_type = TOKEN_OUTREDIR;
//     else
//         return (0);  // Not a redirection character

//     while (input[count] == input[0])
//         count++;

//     new_token = malloc(sizeof(t_lexer));
//     if (!new_token)
//         return (0);  // Handle memory allocation failure

//     new_token->type = token_type;
//     new_token->data = strndup(input, count);  // Store all consecutive redirection characters
//     new_token->next = NULL;
//     new_token->prev = NULL;

//     add_token_to_lexer(lexer, new_token);  // Add the redirection token to the lexer

//     return (count);  // Return the number of processed redirection characters
// }

// int is_space(char c)
// {
//     return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f');
// }

// int handle_pipe(t_lexer **lexer, char *input)
// {
//     t_lexer *new_token;
//     int count = 0;

//     // Count consecutive pipe characters
//     while (input[count] == '|')
//         count++;

//     new_token = malloc(sizeof(t_lexer));
//     if (!new_token)
//         return (0);  

//     new_token->type = TOKEN_PIPE;  
//     new_token->data = strndup(input, count); 
//     new_token->next = NULL;
//     new_token->prev = NULL;

//     add_token_to_lexer(lexer, new_token);  

//     return (count);  
// }

// // int extract_word(t_lexer **lexer, char *input)
// // {
// //     t_lexer *new_token;
// //     int len = 0;
// //     char quote_char = '\0';

    
// //     new_token = malloc(sizeof(t_lexer));
// //     if (!new_token)
// //         return (0);  

     
// //     if (input[len] == '"' || input[len] == '\'')
// //     {
// //         quote_char = input[len];
// //         len++;
        
// //         while (input[len] && input[len] != quote_char)
// //             len++;
// //         if (input[len] == quote_char)
// //             len++; 
// //     }
// //     else
// //     {
        
// //         while (input[len] && !is_space(input[len]) && input[len] != '|' && input[len] != '>' && input[len] != '<')
// //             len++;
// //     }

    
// //     new_token->data = malloc(len + 1);
// //     if (!new_token->data)
// //     {
// //         free(new_token);  
// //         return (0);
// //     }

// //     ft_strncpy(new_token->data, input, len);
// //     new_token->data[len] = '\0';  

// //     new_token->type = TOKEN_WORD;  
// //     new_token->next = NULL;
// //     new_token->prev = NULL;

    
// //     add_token_to_lexer(lexer, new_token);

// //     return (len);  
// // }

int	ft_token(char c, int *singl, int *doubl)
{
	if (c == '\'' && *doubl != -1)
		*singl *= -1;
	if (c == '\"' && *singl != -1)
		*doubl *= -1;
	if (ft_strsrch("()&;<>|\t\v\n ", c) && (*singl != -1 && *doubl != -1))
		return (0);
	else
		return (1);
}

// int	extract_word(t_lexer **lexer, char *len)
// {
// 	int		singl;
// 	int		doubl;
// 	int		i;
// 	char	*str;

// 	i = 0;
// 	singl = 1;
// 	doubl = 1;
// 	while (len[i] && ft_token(len[i], &singl, &doubl))
// 		i++;
// 	str = malloc(i + 1);
// 	if (str == NULL)
// 		return (0);
// 	ft_strncpy(str, len, i);
// 	add_token_to_lexer(lexer, str);
// 	return (i);
// }

// // #include <stdio.h>

// // int check_syntax_errors(t_lexer **lexer)
// // {
// //     t_lexer *current;

// //     if (!*lexer)
// //     {
// //         printf("Syntax error: Empty input\n");
// //         return (1);
// //     }
// //     current = *lexer;

// //     // Check if the first token is valid
// //     if (current->type == TOKEN_PIPE || current->type == TOKEN_INREDIR || current->type == TOKEN_OUTREDIR)
// //     {
// //         printf("Syntax error: Invalid start token\n");
// //         return (1);
// //     }

// //     while (current)
// //     {
// //         if (current->type == TOKEN_PIPE)
// //         {
// //             if (current->next == NULL || current->next->type == TOKEN_PIPE)
// //             {
// //                 printf("Syntax error: Invalid pipe usage\n");
// //                 return (1);
// //             }
// //         }

// //         if (current->type == TOKEN_INREDIR || current->type == TOKEN_OUTREDIR)
// //         {
// //             if (current->next == NULL || current->next->type != TOKEN_WORD)
// //             {
// //                 printf("Syntax error: Redirection not followed by a word\n");
// //                 return (1);
// //             }
// //             // Check for multiple consecutive redirection characters
// //             if (strlen(current->data) > 2)
// //             {
// //                 printf("Syntax error: Invalid redirection usage\n");
// //                 return (1);
// //             }
// //         }

// //         current = current->next;
// //     }

// //     // Check if the last token is valid
// //     current = *lexer;
// //     while (current->next)
// //         current = current->next;

// //     if (current->type == TOKEN_PIPE || current->type == TOKEN_INREDIR || current->type == TOKEN_OUTREDIR)
// //     {
// //         printf("Syntax error: Invalid end token\n");
// //         return (1);
// //     }
// //     return (0);
// // }



// void lexer_analysis(char *input, t_lexer **lexer)
// {
//     int i;

//     i = 0;
//     *lexer = NULL;
//     while (input[i])
//     {
//         if (is_space(input[i]))
//             i++;
//         else if (input[i] == '>' || input[i] == '<')
//             i += handle_redirection(lexer, &input[i]);
//         else if (input[i] == '|')
//             i += handle_pipe(lexer, &input[i]);
//         else
//             i += extract_word(lexer, &input[i]);
//     }

//     // Call check_syntax_errors only if lexer is not empty
//     if (*lexer != NULL)
//     {
//         check_syntax_errors(lexer);
//     }
// }



// apend_in_struct function:

char	*ft_strncpy(char *dest, char *src, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

int	is_whitespace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f')
		return (1);
	return (0);
}

void	*ft_calloc(size_t count, size_t size)
{
	void	*p;

	if (count != 0 && size > SIZE_MAX / count)
		return (0);
	p = malloc(count * size);
	if (p == NULL)
		return (NULL);
	memset(p, 0, count * size);
	return (p);
}

void	ft_lstadd_back(t_lexer **lst, t_lexer *new)
{
	t_lexer	*temp;

	if (!lst || !new)
		return ;
	if (*lst)
	{
		temp = *lst;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = new;
		new->prev = temp;
	}
	else
		*lst = new;
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

void	apend_in_struct(t_lexer **lexer, char *str, int type)
{
	t_lexer	*new_lexer;
	int		len;

	len = ft_strlen(str);
	if (type == TOKEN_INREDIR && len == 2)
		type = TOKEN_HEREDOC;
	else if (type == TOKEN_OUTREDIR && len == 2)
		type = TOKEN_REDIR_APPEND;
	new_lexer = ft_calloc(sizeof(t_lexer), 1);
	if (new_lexer == NULL)
		return ;
	new_lexer->data = str;
	new_lexer->type = type;
	ft_lstadd_back(lexer, new_lexer);
}

#include "minishell.h"

int	ft_outred(t_lexer **lexer, char *len)
{
	int		i;
	char	*str;

	i = 0;
	while (len[i] == '>')
		i++;
	str = malloc(i + 1);
	if (str == NULL)
		return (0);
	ft_strncpy(str, len, i);
	apend_in_struct(lexer, str, TOKEN_OUTREDIR);
	return (i);
}

int	ft_inreder(t_lexer **lexer, char *len)
{
	int		i;
	char	*str;

	i = 0;
	while (len[i] == '<')
		i++;
	str = malloc(i + 1);
	if (str == NULL)
		return (0);
	ft_strncpy(str, len, i);
	apend_in_struct(lexer, str, TOKEN_INREDIR);
	return (i);
}

int	ft_pipe(t_lexer **lexer, char *len)
{
	int		i;
	char	*str;

	i = 0;
	while (len[i] == '|')
		i++;
	str = malloc(i + 1);
	if (str == NULL)
		return (0);
	ft_strncpy(str, len, i);
	apend_in_struct(lexer, str, TOKEN_PIPE);
	return (i);
}

int	ft_word(t_lexer **lexer, char *len)
{
	int		singl;
	int		doubl;
	int		i;
	char	*str;

	i = 0;
	singl = 1;
	doubl = 1;
	while (len[i] && ft_token(len[i], &singl, &doubl))
		i++;
	str = malloc(i + 1);
	if (str == NULL)
		return (0);
	ft_strncpy(str, len, i);
	apend_in_struct(lexer, str, TOKEN_WORD);
	return (i);
}

void	lexer_analysis(char *len, t_lexer **lexer)
{
	int	i;

	i = 0;
	*lexer = NULL;
	while (len[i])
	{
		if (len[i] == '>')
			i += ft_outred(lexer, &len[i]);
		else if (len[i] == '<')
			i += ft_inreder(lexer, &len[i]);
		else if (len[i] == '|')
			i += ft_pipe(lexer, &len[i]);
		else if (is_whitespace(len[i]))
			i++;
		else
			i += ft_word(lexer, &len[i]);
	}
	check_syntax_errors(lexer);
}