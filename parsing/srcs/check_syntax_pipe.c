/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_pipe.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmonacho <wmonacho@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 18:43:39 by wmonacho          #+#    #+#             */
/*   Updated: 2022/11/16 14:33:15 by wmonacho         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

static char	*ft_prev(char *tmp, char *str, int j, int i)
{
	char	*buff;

	tmp = malloc(sizeof(char) * (ft_strlen(str) - (j + i - 1)));
	if (!tmp)
		return (NULL);
	buff = cmd_cpy(tmp, str + j + i, ft_strlen(str) - (j + i - 1));
	buff[ft_strlen(str) - (j + i)] = '\0';
	return (buff);
}

static void	ft_add_pipe(t_shell *minishell, char *tmp
	, int *i, t_node **list_cpy)
{
	t_node	*buff;

	(void) list_cpy;
	ft_dlstadd_back((&(minishell)), ft_dlstnew((void *)tmp));
	buff = ft_dlstnew((void *)ft_strdup("|"));
	if (buff)
		buff->type = 'P';
	ft_dlstadd_back((&(minishell)), buff);
	(*i)++;
}

void	add_last_arg(t_shell *minishell, t_node **list_cpy, int *i, int j)
{
	ft_add_pipe(minishell, minishell->tmpp, i, list_cpy);
	if (minishell->pipe == 0 && minishell->strp[(*i) + j] != '\0')
	{
		minishell->tmpp = ft_prev(minishell->tmpp, minishell->strp, j, (*i));
		ft_dlstadd_back((&(minishell)), ft_dlstnew((void *)minishell->tmpp));
		if (!minishell->tmpp)
			return (free_all_exit(minishell));
	}
}

static int	check_syntax_utils(t_shell *minishell, int j, int i, char *str)
{
	while (--j >= 0 && str[j] == ' ')
	{
		if (j == 0)
		{
			write(2, "minishell: syntax error near unexpected token `|'\n", 50);
			minishell->error = 258;
			return (0);
		}
	}
	while (str[++i] != '\0' && (str[i] == ' ' || str[i] == '|'))
	{
		if (str[i] == '|')
		{
			if (str[i + 1] == '|')
				write(2,
					"minishell: syntax error near unexpected token `||'\n", 51);
			else
				write(2,
					"minishell: syntax error near unexpected token `|'\n", 50);
			minishell->error = 258;
			return (0);
		}
	}
	return (1);
}

int	check_syntax(t_shell *minishell, char *str, int i)
{
	int	j;

	j = i;
	if (i == 0 || str[i + 1] == '\0')
	{
		write(2, "minishell: syntax error near unexpected token `|'\n", 50);
		minishell->error = 258;
		return (0);
	}
	if (str[i + 1] == '|')
	{
		write(2, "minishell: syntax error near unexpected token `||'\n", 51);
		minishell->error = 258;
		return (0);
	}
	if (check_syntax_utils(minishell, j, i, str) == 0)
		return (0);
	return (1);
}
