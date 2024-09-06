#include "minishell.h"

int	check_quotes(char *str)
{
	int i;
	int single_quotes;
	int double_quotes;

	i = 0;
	single_quotes = 0;
	double_quotes = 0;
	while (str[i])
	{
		if (str[i] == '\'' && double_quotes % 2 == 0)
			single_quotes++;
		if (str[i] == '\"' && single_quotes % 2 == 0)
			double_quotes++;
		i++;
	}
	if (single_quotes % 2 == 0 && double_quotes % 2 == 0)
		return (1);
	return (0);
}