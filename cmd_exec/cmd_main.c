/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebrodeur <ebrodeur@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/05 11:14:50 by ebrodeur          #+#    #+#             */
/*   Updated: 2022/10/28 15:04:07 by ebrodeur         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "cmd_include/pipex_bonus.h"
#include "../includes/minishell.h"

void	first_command(char *envp[], t_data *data, t_node *node)
{
	signal(SIGQUIT, &sigtest);
	signal(SIGINT, &sigint_handler_in_process);
	data->first_cmd_pid = fork_creation(data->first_cmd_pid);
	if (data->first_cmd_pid == 0)
	{
		//printf("FIRST COMMAND --> %s\n", node->content);
		if (check_inputfile(data) != 0)
			return ;
		data->env.tab1 = get_path(envp, data, data->env.tab1);
		data->env.param_tab1 = fill_param_tab(node, data, data->env.param_tab1);
		while (data->env.param_tab1[data->size_ptab1])
			data->size_ptab1++;
		check_outfile(data);
		first_cmd_execution(data, envp);
	}
}

void	last_command(char *envp[], t_data *data, t_node *node)
{
	signal(SIGQUIT, &sigtest);
	signal(SIGINT, &sigint_handler_in_process);
	data->last_cmd_pid = fork_creation(data->last_cmd_pid);
	if (data->last_cmd_pid == 0)
	{
		//printf("LAST COMMAND --> %s\n", node->content);
		if (dup2(data->pipefd[data->cmd_nb - 2][READ], STDIN_FILENO) == -1)
		{
			perror("dup2");
			return ;
		}
		check_outfile_last_cmd(data);
		data->env.tab2 = get_path(envp, data, data->env.tab2);
		data->env.param_tab2 = fill_param_tab(node, data, data->env.param_tab2);
		while (data->env.param_tab2[data->size_ptab2])
			data->size_ptab2++;
		last_cmd_execution(data, envp);
	}
}

void	*commands(t_data *data, t_node *node, char *envp[])
{
	int	i;
	int	cmd_id;
	int	*pid;

	i = -1;
	cmd_id = 3;
	data->pipe_id = 1;
	pid = malloc(sizeof(int) * (data->cmd_nb - 2));
	if (!pid)
		return (0);
	while (++i < (data->cmd_nb - 2))
	{
		pid[i] = fork();
		if (pid[i] == -1)
		{
			ft_printf("Error while creating processes\n");
			return (0);
		}
		if (pid[i] == 0)
			command_exec(data, node, envp, cmd_id);
		node = node_rotation(node);
		data->pipe_id++;
	}
	free(pid);
	return (node);
}
