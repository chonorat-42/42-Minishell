/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 11:35:26 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/23 14:00:34 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_builtin_opt(char *str, size_t *i)
{
	size_t j;
	char *result;

	while (str[*i] && is_ws(str[*i]))
		(*i)++;
	if (str[*i] != '-' || (str[*i] == '-' && is_ws(str[*i + 1])))
		return (NULL);
	j = *i + 1;
	while (str[j] && str[j] != ' ')
		j++;
	result = ft_substr(str, *i, j - *i);
	if (!result)
		return ("0");
	*i = j;
	return (result);
}

void	echo_case(char *prompt, int fd)
{
	char	*option;
	int		opt;
	size_t	i;

	i = 5;
	opt = 0;
	option = get_builtin_opt(prompt, &i);
	if (option)
	{
		if (ft_strncmp(option, "-n", ft_strlen(option)) == 0)
		{
			opt = 1;
			while (prompt[i] && !is_ws(prompt[i]))
				i++;
		}
	}
	while (prompt[i] && is_ws(prompt[i]))
		i++;
	if (prompt[i] && opt)
		ft_putstr_fd(&prompt[i], fd);
	else if (prompt[i] && !opt)
		ft_putendl_fd(&prompt[i], fd);
	if (opt)
		return (free(option));
}
