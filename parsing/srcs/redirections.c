/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmonacho <wmonacho@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 19:34:31 by wmonacho          #+#    #+#             */
/*   Updated: 2022/11/17 19:01:03 by wmonacho         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

int	parse_redirections(t_shell *minishell)
{
	int		size;
	t_node	*list_cpy;
	char	*str;
	int		j;

	j = 0;
	size = minishell->nbr_pipe + 1;
	init_var_redirection(minishell);
	list_cpy = minishell->head;
	while (list_cpy && list_cpy != NULL)
	{
		if (list_cpy->type == 'P')
			j++;
		str = (char *)(list_cpy->content);
		minishell->mod = -1;
		if (sorting_loop(minishell, str, &list_cpy, j) == 0)
			return (0);
		if (list_cpy != NULL)
			list_cpy = list_cpy->next;
	}
	return (1);
}
