/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_unset_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebrodeur <ebrodeur@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 13:02:44 by ebrodeur          #+#    #+#             */
/*   Updated: 2022/09/19 13:26:20 by ebrodeur         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	copy_var_after_index(int i, int j, t_mini_data *data)
{
	data->unset_env[i] = malloc(sizeof(char) * ft_strlen(data->env[i + 1]) + 1);
	if (!data->unset_env[i])
	{
		free_tab(data->unset_env, i);
		return (0);
	}
	while (data->env[i + 1][++j])
		data->unset_env[i][j] = data->env[i + 1][j];
	data->unset_env[i][j] = '\0';
	return (1);
}

int	copy_var_before_index(int i, int j, t_mini_data *data)
{
	data->unset_env[i] = malloc(sizeof(char) * ft_strlen(data->env[i]) + 1);
	if (!data->unset_env[i])
	{
		free_tab(data->unset_env, i);
		return (0);
	}
	while (data->env[i][++j])
		data->unset_env[i][j] = data->env[i][j];
	data->unset_env[i][j] = '\0';
	return (1);
}

int	unset_var(int index, t_mini_data *data)
{
	int	i;
	int	j;

	i = 0;
	if (check_var(data->env[index], data->var_name))
	{
		if (malloc_and_cpy(data, i, j, index) == 1)
			return (1);
		data->envp_size--;
		return (0);
	}
	return (0);
}

int	malloc_and_cpy(t_mini_data *data, int i, int j, int index)
{
	data->unset_env = malloc(sizeof(char *) * (data->envp_size - 1));
	if (!data->unset_env)
		return (1);
	while (i < data->envp_size - 1)
	{
		j = -1;
		if (i >= index)
		{
			copy_var_after_index(i, j, data);
			i++;
		}
		else
		{
			copy_var_before_index(i, j, data);
			i++;
		}
	}
	return (0);
}
