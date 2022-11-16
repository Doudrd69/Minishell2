/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_execution_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebrodeur <ebrodeur@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 12:39:41 by ebrodeur          #+#    #+#             */
/*   Updated: 2022/11/16 14:40:32 by ebrodeur         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_for_append(t_node *infile_tmp)
{
	if (infile_tmp == NULL)
		return (1);
	else if (infile_tmp->type == 'C' && infile_tmp->next == NULL)
		return (1);
	else if (infile_tmp->type == 'A' && infile_tmp->next == NULL)
		return (0);
	while (infile_tmp->next != NULL)
	{
		if (infile_tmp->type == 'A')
			return (0);
		infile_tmp = infile_tmp->next;
	}
	return (1);
}

int	search_hd(t_node *tmp, t_shell *parse, t_node **tab)
{
	int	index;

	index = 0;
	while (index < parse->infile_size)
	{
		tmp = tab[index];
		if (tmp == NULL)
			index++;
		else
		{
			while (tmp->next != NULL && tmp->type != 'A')
				tmp = tmp->next;
			if (tmp && tmp->type == 'A')
				break ;
			index++;
		}
	}
	return (index);
}

t_node	*infile_rotation(t_data *data, t_node *tmp, t_node **i_tmp)
{
	if (tmp && tmp->type == 'C' && tmp->next == NULL)
	{
		data->hd.index++;
		tmp = i_tmp[data->hd.index];
	}
	else if (tmp && tmp->type == 'A' && tmp->next == NULL)
		tmp = tmp->next;
	else
	{
		if (tmp)
		{
			while (tmp->next != NULL && (tmp->type != 'A'))
				tmp = tmp->next;
		}
	}
	if (tmp == NULL)
	{
		data->hd.index++;
		tmp = i_tmp[data->hd.index];
	}
	if (tmp == NULL || tmp->type != 'A')
		tmp = infile_rotation(data, tmp, i_tmp);
	return (tmp);
}

t_node	*rotation_after_exec(t_node *tmp, t_data *data, t_node **in, t_shell *s)
{
	if (tmp && tmp->next != NULL && tmp->next->type == 'A')
		tmp = tmp->next;
	else if (tmp && tmp->next != NULL && tmp->next->type == 'C')
	{
		tmp = tmp->next;
		if (tmp->next == NULL)
		{
			data->hd.index++;
			tmp = in[data->hd.index];
		}
		else if (tmp->next != NULL)
		{
			while (tmp->next != NULL && (tmp->type != 'A'))
				tmp = tmp->next;
		}
	}
	else
	{
		if (data->hd.index < s->infile_size)
		{
			data->hd.index++;
			tmp = in[data->hd.index];
		}
	}
	return (tmp);
}

int	heredoc_loop(t_data *data, t_node **infile_tmp, t_shell *parse, int ptr)
{
	int		i;
	t_node	*tmp;

	i = -1;
	tmp = infile_tmp[data->hd.index];
	tmp = search_first_hd(data, tmp, parse, infile_tmp);
	while (++i < data->heredoc_nb)
	{
		data->hd_pid[i] = fork();
		if (data->hd_pid[i] == -1)
		{
			dprintf(2, "RETURN 6\n");
			return (1);
		}
		if (data->hd_pid[i] == 0)
			heredoc(data, tmp);
		waitpid(data->hd_pid[i], &ptr, 0);
		if (ptr != 0)
			return (1);
		if (tmp == NULL && (data->hd.index < parse->infile_size))
		{
			data->hd.index++;
			tmp = infile_tmp[data->hd.index];
		}
		if (tmp == NULL || tmp->type != 'A')
			tmp = infile_rotation(data, tmp, infile_tmp);
		tmp = rotation_after_exec(tmp, data, infile_tmp, parse);
		data->hd_pipe_id++;
		data->hd_id++;
	}
	return (0);
}
