/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebrodeur <ebrodeur@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 11:11:11 by ebrodeur          #+#    #+#             */
/*   Updated: 2022/10/19 16:48:06 by ebrodeur         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"
#include "cmd_exec/cmd_include/pipex_bonus.h"
#include <readline/readline.h>
#include <readline/history.h>

int		export_exec(t_mini_data *mini_data, t_data *data);
int		unset_exec(t_mini_data *mini_data, t_data *data);
void	exec_main(t_data *data, char *envp[]);
void	cmd_exec(t_data *data, char **envp);
void	heredoc_main(t_data *data);

int		p_status;

void	init_main(t_mini_data *mini_data, t_data *data, char **envp)
{
	mini_data->name = "TEST";		//on recup ca dans le parsing
	mini_data->value = "issou";
	mini_data->var_export = "TEST=onestlahein";
	mini_data->path = "..";
	mini_data->str = "P_STATUS : $? == $HOME et $TERM issou$";//$LOGNAM on est $HOM$?E la $ISS$?OU hein cha$kal $TERM $?
	mini_data->echo_arg = 0;
	mini_data->var_name = "";
	mini_data->hd_limit = "on est la hein";
	mini_data->env = envp;
	mini_data->no_env_check = 0;	//utils epour la creation du oldpwd
	mini_data->first_cd_check = 0;
	mini_data->unset_env_check = 0;
	mini_data->new_env_check = 0;
	mini_data->p_status = &p_status;
	mini_data->echo_sq_check = 0;
	mini_data->oldpwd_if = 0;
	data->envp = envp;
	p_status = 0;
	return ;
}

void	cmd_exec_init(t_data *data)
{
	data->input_fd = STDIN_FILENO;
	data->output_fd = STDOUT_FILENO;

	data->p_status = &p_status;

	data->hd_pipe_id = 0;
	data->hd_id = 0;

	data->cmd_nb = 3;
	data->heredoc_nb = 2;
	data->check_hd = 1;

	data->hd.delimiter_quotes = 0;

	data->exec.infile_fd = "infile.txt";
	data->exec.outfile_fd = "outfile.txt";
	data->exec.first_cmd_test = "cat -e";
	data->exec.last_cmd_test = "rev";

	data->exec.first_cmd_squotes_check = 0;
	data->exec.infile_check = 0;
	data->exec.outfile_check = 0;
	data->exec.last_cmd_outfile_check = 0;
	data->exec.pipe_check = 1;
	return ;
}

void	envp_check(t_mini_data *mini_data, t_data *data, char **envp, int envpsize)
{
	if (envp[0] != NULL)
	{
		while (envp[envpsize])
			envpsize++;
		mini_data->envp_size = envpsize;
		data->envp_size = envpsize;
	}
	else
	{
		if (export_no_env(mini_data) == 1)
			exit(1);
		envp = mini_data->no_env;
		mini_data->env = envp;
		data->envp = envp;
		envpsize = 3;
		mini_data->envp_size = envpsize;
		data->envp_size = envpsize;
	}
	return ;
}

int main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	struct		sigaction sa;
	t_mini_data	mini_data;
	t_data		data;
	int			builtin_cmd_nb = 5;
	int			envpsize = 0;
	int			check;
	int			i;
	int			(*builtins[5])(t_mini_data *data);
	char		*builtins_name[] = {
		"cd",
		"echo",
		"env",
		"pwd",
		"exit"
	};
	char		*input;

	builtins[0] = &mini_cd;			//OK + penser a enlever les printf
	builtins[1] = &mini_echo;		//OK
	builtins[2] = &mini_env;		//OK
	builtins[3] = &mini_pwd;		//OK mais probleme avec buff
	builtins[4] = &mini_exit;		//A FAIRE

	init_main(&mini_data, &data, envp);
	sa.sa_handler = SIG_IGN;
	envp_check(&mini_data, &data, envp, envpsize);
	while (1)
	{
		signal(SIGINT, &sighandler);
		sigaction(SIGQUIT, &sa, NULL);
		input = readline("minishell$ ");
		if (input && *input)
			add_history (input);
		eof_handler(input);
		check = 0;
		i = 0;
		data.envp_size = mini_data.envp_size;
		while (i < builtin_cmd_nb)
		{
			if (input)
			{
				if (ft_strcmp(builtins_name[i], input) == 0)
				{
					if ((*builtins[i])(&mini_data) == 1)
					{
						printf("P_STATUS fail : %d\n", *mini_data.p_status);
						check = 1;
						break ;
					}
					check = 1;
					break;
				}
			}
			i++;
		}
		if (ft_strcmp(input, "export") == 0)
		{
			export_exec(&mini_data, &data);
			check = 1;
		}
		if (ft_strcmp(input, "unset") == 0)
		{
			unset_exec(&mini_data, &data);
			check = 1;
		}
		if (check == 0)
			cmd_exec(&data, data.envp);
		free(input);
	}
}

void	cmd_exec(t_data *data, char **envp)
{
	cmd_exec_init(data);
	int pipe_nb = 0;
	heredoc_main(data);							//exec des HD
	if (*data->p_status == 2)
	{
		printf("CA MARCHE AHAHAHA\n");
		close_hd_pipe(data, data->heredoc_nb - 1);
		free_inttab(data->hd_pipefd, data->heredoc_nb - 1);
		free(data->hd_pid);
		return ;
	}
	pipe_nb = pipe_creation(data);				//On cree les pipe
	exec_main(data, envp);						//exec des commandes
	if (data->check_hd == 1)					//on close les pipes des Heredocs
	{
		close_hd_pipe(data, data->heredoc_nb - 1);
		free_inttab(data->hd_pipefd, data->heredoc_nb - 1);
	}
	if (data->exec.pipe_check > 0)				//on close les pipes des process
		close_pipe(data, (pipe_nb - 1));
	while (wait(NULL) != -1)					//on attend les process
		;
	if (data->check_hd > 0)
		free(data->hd_pid);
	return ;
}
//dans HD ---> CTRL-C retourne au prompt sans executer le HD