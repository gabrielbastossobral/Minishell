#include "minishell.h"

static char	*filler(char *input, int pos)
{
	char	*new;
	int		i;
	int		j;

	new = ft_calloc(ft_strlen(input) + 3, sizeof(char));
	i = -1;
	j = -1;
	while (++j < pos)
		new[j] = input[++i];
	i++;
	new[j++] = ' ';
	new[j++] = input[i++];
	if (input[i] == input[i - 1] && input[i] != '|')
		new[j++] = input[i++];
	new[j++] = ' ';
	while (input[i])
		new[j++] = input[i++];
	new[j] = '\0';
	input = free_ptr(input);
	return (new);
}

char	**lexer(char *input)
{
	int		i;
	int		quotes;
	char	*temp;
	char	**ret;

	i = -1;
	quotes = 0;
	temp = ft_strdup(input);
	while (temp && temp[++i])
	{
		if ((temp[i] == '|' || temp[i] == '>' || temp[i] == '<' ) && !quotes)
		{
			temp = filler(temp, i);
			i = i + 2;
		}
		else if (temp[i] == '\"' || temp[i] == '\'')
			quotes = check_quotes(temp[i], quotes);
	}
	if (quotes)
		temp = free_ptr(temp);
	ret = ft_split(temp, ' ');
	temp = free_ptr(temp);
	return (ret);
}
