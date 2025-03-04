#include "../includes/minishell.h"

char *expand_var(char *str, char **envp)
{
    int i;
    char *result;
    char *var_name;
    int quotes;

    i = 0;
    result = ft_strdup("");
    quotes = 0;
    if (str[0] == '\'' && str[strlen(str) - 1] == '\'')
        return ft_strdup(str);
    while (str[i])
    {
        process_quotes(str[i], &quotes);
        if (str[i] == '$' && quotes != 2)
        {
            var_name = extract_var_name(str, &i);
            result = append_var_value(result, var_name, envp);
            free(var_name);
        }
        else
            result = append_char_to_result(result, str[i++]);
    }
    return (result);
}

void expand(t_data *data)
{
    t_token *token;
    char *expanded_value;

    token = data->tokens;
    while (token)
    {
        if (ft_strchr(token->value, '$'))
        {
            expanded_value = expand_var(token->value, data->envp);
            free(token->value);
            token->value = expanded_value;
        }
        token = token->next;
    }
}
