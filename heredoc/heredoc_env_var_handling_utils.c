/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_env_var_handling_utils.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebrodeur <ebrodeur@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 13:39:04 by ebrodeur          #+#    #+#             */
/*   Updated: 2022/11/17 20:05:41 by ebrodeur         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_special_char(char c, int size)
{
	if (c == '$' && size != 0)
		return (1);
	if (c == '"' && size != 0)
		return (1);
	if (c == '\'' && size != 0)
		return (1);
	if (c == '\\' && size != 0)
		return (1);
	return (0);
}

int	check_special_char_second_loop(char c)
{
	if (c == '\0')
		return (1);
	if (c == '"' || c == '\'')
		return (1);
	if (c == '\\')
		return (1);
	return (0);
}

int	backslash_check(t_data *data, char *str, int i)
{
	if (str[i] == '\\' && str[i + 1] == '$')
	{
		data->hd.bkslash_check = 1;
		i++;
	}
	if (str[i] == '\\' && str[i + 1] == '\\')
		i++;
	return (i);
}

int	cpvhd_specific_cases(t_data *data, char *str, int i, int output_fd)
{
	if (str[i + 1] == ' ')
	{
		write(output_fd, &str[i], 1);
		i++;
	}
	else
		i = print_var_util(data, str, i, output_fd);
	while (str[i] == '$')
	{
		if (check_special_char_second_loop(str[i + 1]) == 1)
			break ;
		i = print_var_util(data, str, i, output_fd);
	}
	return (i);
}

void	iterate_outfile_hd(t_shell *parse, int i)
{
	while (parse->tab_outfile && parse->tab_outfile[i] != NULL)
	{
		if (parse->tab_outfile[i]->next == NULL)
		{
			if (parse->tab_outfile[i]->type == 'C')
				(open(parse->tab_outfile[i]->content,
						O_WRONLY | O_TRUNC | O_CREAT, 0666));
			else
				(open(parse->tab_outfile[i]->content,
						O_WRONLY | O_APPEND | O_CREAT, 0666));
		}
		if (parse->tab_outfile[i]->type == 'C')
			open(parse->tab_outfile[i]->content, O_WRONLY
				| O_TRUNC | O_CREAT, 0666);
		else
			open(parse->tab_outfile[i]->content, O_WRONLY
				| O_APPEND | O_CREAT, 0666);
		if (parse->tab_outfile[i]->next == NULL)
			i++;
		else
			parse->tab_outfile[i] = parse->tab_outfile[i]->next;
	}
	return ;
}
