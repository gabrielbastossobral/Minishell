#include "../includes/minishell.h"

static int check_next(t_data *data, t_token *token, int type)
{
    if (type == PIPE && token->next && token->next->type == PIPE)
    {
        ft_printf("minishell: syntax error near unexpected token %s\n", token->next->value);
        data->exit_error = 2;
        return (2);
    }
    else if ((type == REDIRECT && token->next == NULL) || \
    (type == REDIRECT && (token->next->type == PIPE || \
    token->next->type == REDIRECT || \
    token->next->type == HEREDOC)))
    {
        ft_printf("minishell: syntax error near unexpected token %s\n", token->value);
        data->exit_error = 2;
        return (2);
    }
    return (0);
}

int syntax_checker(t_data *data)
{
    t_token *tmp;

    tmp = data->tokens;
    while (tmp)
    {
        if (tmp->type == PIPE)
        {
            if (check_next(data, tmp, PIPE))
                return (2);
        }
        else if ((tmp->type == REDIRECT))
        {
            if (check_next(data, tmp, REDIRECT))
                return (2);
        }
        else if (ft_strchr(tmp->value, '\\'))
        {
            ft_printf("minishell: syntax error, token `\\' is not accepted\n");
            data->exit_error = 2;
            return (2);
        }
        tmp = tmp->next;
    }
    return (0);
}
