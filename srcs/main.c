/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcosta-m <gcosta-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 10:09:47 by gabastos          #+#    #+#             */
/*   Updated: 2025/02/24 12:10:02 by gcosta-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_matrix(char **matrix)
{
	int	i;

	i = -1;
	while (matrix[++i])
		free(matrix[i]);
	free(matrix);
}

int	ft_isspace(char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

// char	**get_envp(char **envp)
// {
// 	int		i;
// 	char	**new;

// 	i = 0;
// 	while (envp[i])
// 		i++;
// 	new = ft_calloc(i + 1, sizeof(char *));
// 	if (!new)
// 		return (0);
// 	i = -1;
// 	while (envp[++i])
// 	{
// 		new[i] = ft_strdup(envp[i]);
// 		if (!new[i])
// 		{
// 			free_matrix(new);
// 			return (0);
// 		}
// 	}
// 	return (new);
// }

// void	save_env_var(char *line, int *count, t_data *data, int env)
// {
// 	int	equal;
// 	int	end;
// 	int	i;

// 	if (line[0] == '"' || line[0] == '\'')
// 		return ;
// 	i = get_end_var(line, env, &end, &equal);
// 	if (i == -1)
// 		return ;
// 	while (ft_isspace(line[i]))
// 		(*count) += end;
// 	if (line[i] != 0 && line[i] != ';')
// 		return ;
// 	if (equal)
// 		create_new_var(data, line, end, equal);
// }

// void	init_vars(t_data *data, char **envp)
// {
// 	int	count;
// 	int	index;

// 	count = 0;
// 	index = -1;
// 	while (envp[++index])
// 		save_env_var(envp[index], &count, data, 1);
// }

// static void	init(t_data *data, char **envp)
// {
// 	data->vars = 0;
// 	data->envp = get_envp(envp);
// 	init_vars(&data, envp);
// }

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	(void)envp;
	//t_data	data;

	if (argc != 1 || argv[1])
		handle_erros("Usage: ./minishell", 0, NULL);
	//init(&data, envp);
	while (1)
	{
		line = readline(GREEN"☯️💰🤡$MINI$HELL_DE_VILÃO$🤡💰☯️ $"RESET);
		if (!line)
			break ;
	}
	return (0);
}
