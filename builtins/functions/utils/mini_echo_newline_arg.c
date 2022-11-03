/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_echo_newline_arg.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebrodeur <ebrodeur@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 19:36:13 by ebrodeur          #+#    #+#             */
/*   Updated: 2022/10/28 19:48:26 by ebrodeur         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	no_args(t_node *node)
{
	if ((ft_strncmp(node->content, "echo", 4) == 0))
	{
		write(1, "\n", 1);
		return (0);
	}
	return (1);
}

int	newline_arg(t_mini_data *data)
{
	if (data->echo_arg == 0)
		write(1, "\n", 1);
	return (0);
}