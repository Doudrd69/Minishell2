/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unstack_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmonacho <wmonacho@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 09:26:32 by wmonacho          #+#    #+#             */
/*   Updated: 2022/11/16 10:07:04 by wmonacho         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"
#include "../../includes/minishell.h"

static int	last_one(t_shell *minishell, t_node **list)
{
	if ((*list)->prev == NULL && (*list)->next == NULL)
	{
		printf("lst_one\n");
		if ((*list)->content != minishell->cmd)
			free((*list)->content);
		free((*list));
		(*list) = NULL;
		minishell->head = NULL;
		return (1);
	}
	return (0);
}

static int	next_null(t_shell *minishell, t_node **list)
{
	t_node	*cpy;

	if ((*list)->prev != NULL && (*list)->next == NULL)
	{
		cpy = (*list);
		(*list) = (*list)->prev;
		(*list)->next = NULL;
		if (cpy->content != minishell->cmd)
			free(cpy->content);
		free(cpy);
		return (1);
	}
	return (0);
}

static int	in_the_middle(t_shell *minishell, t_node **list)
{
	t_node	*cpy;

	if ((*list)->prev != NULL && (*list)->next != NULL)
	{
		cpy = (*list);
		(*list) = (*list)->prev;
		(*list)->next = (*list)->next->next;
		(*list)->next->next->prev = (*list);
		if (cpy->content != minishell->cmd)
			free(cpy->content);
		free(cpy);
		return (1);
	}
	return (0);
}

static int	the_first(t_shell *minishell, t_node **list)
{
	t_node	*cpy;

	if ((*list)->prev == NULL && (*list)->next != NULL)
	{
		cpy = (*list);
		(*list) = (*list)->next;
		(*list)->prev = NULL;
		minishell->head = (*list);
		if (cpy->content != minishell->cmd)
			free(cpy->content);
		free(cpy);
		return (1);
	}
	return (0);
}

void	unstack_list(t_shell *minishell, t_node **list)
{
	if (last_one(minishell, list) == 1)
		return ;
	if (next_null(minishell, list) == 1)
		return ;
	if (in_the_middle(minishell, list) == 1)
		return ;
	if (the_first(minishell, list) == 1)
		return ;
}
