/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   no_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebrodeur <ebrodeur@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 15:03:56 by ebrodeur          #+#    #+#             */
/*   Updated: 2022/11/16 12:11:39 by ebrodeur         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	create_oldpwd(t_data *data)
{
	char	*str;

	str = malloc(sizeof(char) * (ft_strlen(data->oldpwd) + 8));
	if (!str)
		return (1);
	ft_strlcpy(str, "OLDPWD=", 7, 0);
	ft_strlcpy(&str[7], data->oldpwd, ft_strlen(data->oldpwd), 1);
	mini_export(data, str);
	data->envp = data->new_env;
	data->first_cd_check = 1;
	free(str);
	return (0);
}

int	check_oldpwd(t_data *data)
{
	if (data->first_cd_check == 0)
	{
		if (create_oldpwd(data) == 1)
			return (1);
	}
	return (0);
}

int	check_malloc(t_data *data, int i)
{
	if (!data->no_env[i])
	{
		free_tab(data->no_env, i);
		return (1);
	}
	return (0);
}

int	export_no_env(t_data *data)
{
	char	*cwd;
	char	*prog;

	prog = "_=/usr/bin/env";
	data->no_env = malloc(sizeof(char *) * 4);
	if (!data->no_env)
		return (1);
	cwd = getcwd(data->buff, BUF_SIZE);
	data->no_env[0] = malloc(sizeof(char) * ft_strlen(cwd) + 5);
	if (check_malloc(data, 0) == 1)
		return (1);
	ft_strlcpy(data->no_env[0], "PWD=", 4, 0);
	ft_strlcpy(&data->no_env[0][4], cwd, ft_strlen(cwd), 1);
	data->no_env[1] = malloc(sizeof(char) * ft_strlen("SHLVL=1") + 1);
	if (check_malloc(data, 1) == 1)
		return (1);
	ft_strlcpy(data->no_env[1], "SHLVL=1", ft_strlen("SHLVL=1"), 1);
	data->no_env[2] = malloc(sizeof(char) * ft_strlen("_=/usr/bin/env") + 1);
	if (check_malloc(data, 2) == 1)
		return (1);
	ft_strlcpy(data->no_env[2], prog, ft_strlen(prog), 1);
	data->no_env[3] = NULL;
	return (0);
}
