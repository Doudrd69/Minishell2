/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_execution_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebrodeur <ebrodeur@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 09:43:54 by ebrodeur          #+#    #+#             */
/*   Updated: 2022/11/16 10:17:32 by ebrodeur         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_and_exit_builtin(char **arg_tab, char **param, int p_size)
{
	int size = 0;

	free_tab(param, p_size);
	while (arg_tab[size])
		size++;
	free_tab(arg_tab, size - 1);
	exit (0);
}

void	exec_cmd(char **tab, char **param, t_data *data,
	int (*builtins[7])(t_data *, t_node *))
{
	int	i;

	(void)builtins;
	(void)data;
	i = -1;
	if (tab == NULL)
	{
		write(2, "PATH not found : can't execute command : ", 41);
		write(2, param[0], ft_strlen(param[0]));
		write(2, "\n", 1);
		exit(1);
	}
	while (tab[++i])
	{
		if (access(tab[i], X_OK) == 0)
		{
			if (execve(tab[i], param, data->envp) == -1)
			{
				perror("execve");
				exit(errno);
			}
		}
	}
	if (ft_strncmp(param[0], "exit", 4) == 0)
		exit(3);
	exit_cmd_not_found(param);
}

void	exec_cmd_path(t_data *data, char *envp[])
{
	if (data->exec.pipe_check == 1)
		close_pipe(data, (data->cmd_nb - 2));
	if (data->exec.infile_check == 1)
		close(data->input_fd);
	if (data->exec.outfile_check == 1)
		close(data->output_fd);
	if (data->hd_pipefd)
		close_hd_pipe(data, data->heredoc_nb - 1);
	check_file(data->env.param_tab1[0]);
	if (execve(data->env.param_tab1[0], data->env.param_tab1, envp) == -1)
		perror("execve");
}

void	first_cmd_execution(t_data *data, t_node *node,
	int (*builtins[7])(t_data *, t_node *), int g)
{
	if (access(data->env.param_tab1[0], X_OK) == 0)
		exec_cmd_path(data, data->envp);
	else
	{
		if (data->env.tab1 != NULL)
			data->env.tab1 = join_arg(data->env.param_tab1, data->env.tab1);
		if (data->exec.pipe_check == 1)
			close_pipe(data, (data->cmd_nb - 2));
		if (data->exec.infile_check == 1)
			close(data->input_fd);
		if (data->exec.outfile_check == 1)
			close(data->output_fd);
		if (data->hd_pipefd)
			close_hd_pipe(data, data->heredoc_nb - 1);
		check_file(data->env.param_tab1[0]);
		if (builtins_loop(data->builtins_name, builtins, node, data, &g) == 0)
			free_and_exit_builtin(data->env.tab1, data->env.param_tab1,
				data->size_ptab1 - 1);
		exec_cmd(data->env.tab1, data->env.param_tab1, data, builtins);
	}
}

void	last_cmd_execution(t_data *data, t_node *node,
	int (*builtins[7])(t_data *, t_node *), int g)
{
	if (access(data->env.param_tab2[0], X_OK) == 0)
	{
		close_pipe(data, (data->cmd_nb - 2));
		check_file(data->env.param_tab2[0]);
		close(data->output_fd);
		if (execve(data->env.param_tab2[0], data->env.param_tab2, data->envp) == -1)
			perror("execve");
	}
	else
	{
		if (data->env.tab2 != NULL)
			data->env.tab2 = join_arg(data->env.param_tab2, data->env.tab2);
		if (data->exec.pipe_check == 1)
			close_pipe(data, (data->cmd_nb - 2));
		if (data->exec.outfile_check == 1)
			close(data->output_fd);
		if (data->exec.infile_check == 1)
			close(data->input_fd);
		if (data->hd_pipefd)
			close_hd_pipe(data, data->heredoc_nb - 1);
		check_file(data->env.param_tab2[0]);
		if (builtins_loop(data->builtins_name, builtins, node, data, &g) == 0)
			free_and_exit_builtin(data->env.tab2, data->env.param_tab2,
				data->size_ptab2 - 1);
		exec_cmd(data->env.tab2, data->env.param_tab2, data, builtins);
	}
}

void	cmd_execution(t_data *data, int pipe_id, t_node *node,
	int (*builtins[7])(t_data *, t_node *), int g)
{
	if (access(data->env.param_tab3[0], X_OK) == 0)
	{
		close_pipe(data, pipe_id);
		check_file(data->env.param_tab3[0]);
		if (execve(data->env.param_tab3[0], data->env.param_tab3, data->envp) == -1)
			perror("execve");
	}
	else
	{
		if (data->env.tab3 != NULL)
			data->env.tab3 = join_arg(data->env.param_tab3, data->env.tab3);
		if (pipe_id != (data->cmd_nb - 2))
			close_pipe(data, pipe_id + 1);
		else
			close_pipe(data, pipe_id);
		if (data->input_fd > 0)
			close(data->input_fd);
		if (data->hd_pipefd)
			close_hd_pipe(data, data->heredoc_nb - 1);
		check_file(data->env.param_tab3[0]);
		if (builtins_loop(data->builtins_name, builtins, node, data, &g) == 0)
			free_and_exit_builtin(data->env.tab3, data->env.param_tab3,
				data->size_ptab3 - 1);
		exec_cmd(data->env.tab3, data->env.param_tab3, data, builtins);
	}
}
