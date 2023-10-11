/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 18:13:13 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/24 18:24:53 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	are_all_quotes_closed(char *str)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '\"'))
		{
			j = i + 1;
			while (str[j] && str[j] != str[i])
				j++;
			if (!str[j])
				return (0);
			i = j;
		}
		i++;
	}
	return (1);
}

void add_quotes_input(t_mshell *shell, int *nl_count)
{
	char		*line;
	char		*join;
	char		*temp;

	ft_dprintf(STDOUT_FILENO, ">");
	line = get_next_line(0);
	if (!line)
		return (free_struct(shell), exit(1));
	if (*nl_count == 0)
	{
		temp = ft_strjoin(shell->input, "\n");
		(*nl_count)++;
	}
	else
		temp = ft_strdup(shell->input);
	if (!temp)
		return (free(line), free_struct(shell), exit(EXIT_FAILURE));
	join = ft_strjoin(temp, line);
	if (!join)
		return (free(line), free(temp), free_struct(shell), exit(EXIT_FAILURE));
	free(temp);
	free(line);
	free(shell->input);
	shell->input = join;
}

void    parsing(t_mshell *shell)
{
	int	nl_count;

	nl_count = 0;
	while (!are_all_quotes_closed(shell->input))
		add_quotes_input(shell, &nl_count);
    if (shell->input[0] != '\0')
		{
			if (expand(shell, shell->input) == 1)
                return (free(shell->input), get_input_loop(shell));
		}
    else
    {
        free(shell->input);
        shell->input = NULL;
        get_input_loop(shell);
    }	
}
