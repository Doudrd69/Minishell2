/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebrodeur <ebrodeur@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 11:33:22 by ebrodeur          #+#    #+#             */
/*   Updated: 2022/10/19 11:36:14 by ebrodeur         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	new_tab_malloc_loop(t_mini_data *data, int i, int old_tab_size, char **envp)
{
	if (i == old_tab_size)
	{
		data->new_env[i] = malloc(sizeof(char) * ft_strlen(envp[i - 1]) + 1);
		if (!data->new_env[i])
			free_tab(data->new_env, i);
		i++;
	}
	else
	{
		data->new_env[i] = malloc(sizeof(char) * ft_strlen(envp[i]) + 1);
		if (!data->new_env[i])
			free_tab(data->new_env, i);
		i++;
	}
	return (i);
}

char	**newtab_malloc(t_mini_data *data, int old_size, char **envp, char *var)
{
	int	i;

	i = 0;
	while (i < (old_size + 1))
	{
		if (i == old_size - 1)
		{
			data->new_env[i] = malloc(sizeof(char) * (ft_strlen(var) + 2));
			if (!data->new_env[i])
				return (free_tab(data->new_env, i));
			i++;
		}
		else
			i = new_tab_malloc_loop(data, i, old_size, envp);
	}
	return (data->new_env);
}

char	**new_var_tab_copy(t_mini_data *data, int i, char *name, char *value)
{
	int	j;
	int	k;

	j = 0;
	k = 0;
	while (name[j])
	{
		data->new_env[i][j] = name[j];
		j++;
	}
	data->new_env[i][j] = '=';
	j++;
	while (value[k])
	{
		data->new_env[i][j] = value[k];
		k++;
		j++;
	}
	data->new_env[i][j] = '\0';
	return (data->new_env);
}

char	**new_tab_copy(t_mini_data *data, char *envp[], int i, int old_tab_size)
{
	int	j;

	j = 0;
	if (i == old_tab_size)
	{
		while (envp[i - 1][j])
		{
			data->new_env[i][j] = envp[i - 1][j];
			j++;
		}
	}
	else
	{
		while (envp[i][j])
		{
			data->new_env[i][j] = envp[i][j];
			j++;
		}
	}
	data->new_env[i][j] = '\0';
	return (data->new_env);
}

void	copy_loop(t_mini_data *data, char *var_export)
{
	int	i;

	i = 0;
	while (i < (data->envp_size + 1))
	{
		if (i == data->envp_size - 1)
		{
			ft_strlcpy(data->new_env[i], var_export, ft_strlen(var_export), 1);
			i++;
		}
		else
		{
			data->new_env = new_tab_copy(data, data->env, i, data->envp_size);
			i++;
		}
	}
	return ;
}
