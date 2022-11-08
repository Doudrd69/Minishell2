/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebrodeur <ebrodeur@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/05 12:22:52 by ebrodeur          #+#    #+#             */
/*   Updated: 2022/11/08 12:56:09 by ebrodeur         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H
# define READ 0
# define WRITE 1

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../../ft_printf/includes/ft_printf.h"
# include "../../ft_printf/includes/ft_sort_params.h"
# include "../../includes/minishell.h"
# include "../../parsing/parsing.h"
# include "../../parsing/libft/libft.h"

typedef struct heredoc
{
	char	*env_var;
	char	*env_var_value;
	char	*limiter;
	int		var_length;
	int		delimiter_quotes;
	int		hd_pid;
	int		position;
	int		bkslash_check;
	int		check;
	int		flag;
	int		index;
	char	**envp;
}	t_heredoc;

typedef struct exec
{
	int		last_cmd_outfile_check;
	int		first_cmd_squotes_check;
	int		infile_check;
	int		outfile_check;
	int		pipe_check;
	int		append_check;
	char	*infile_fd;
	char	*outfile_fd;
	char	*first_cmd_test;
	char	*last_cmd_test;
}	t_exec;

typedef struct env
{
	char	*path;
	char	*arg1;
	char	*arg2;
	char	**tab1;
	char	**tab2;
	char	**tab3;
	char	**param_tab1;
	char	**param_tab2;
	char	**param_tab3;
}	t_env;

typedef struct data
{
	int			cmd_nb;
	int			pid;
	int			**pipefd;
	int			**hd_pipefd;
	int			input_fd;
	int			output_fd;
	int			index;
	int			pipe_id;
	int			first_cmd_pid;
	int			last_cmd_pid;
	int			envp_size;
	int			check_hd;
	int			heredoc_nb;
	int			hd_pipe_id;
	int			hd_id;
	int			*hd_pid;
	int			*p_status;
	int			lst_size;
	int			nb_of_args;
	int			size_ptab1;
	int			size_ptab2;
	int			size_ptab3;
	char		**envp;
	char		*home_path;
	t_heredoc	hd;
	t_env		env;
	t_exec		exec;
}	t_data;

/* HEREDOC */
void	eof_handler_hd(t_data *data, char *input, int output_fd);
void	heredoc(t_data *data, t_node *tmp);
void	close_hd_pipe(t_data *data, int i);
void	output_redirection(t_data *data);
void	print_heredoc(int output_fd);
void	sighandler_hd(int signum);

char	*var_found(t_data *data, char *envp[], char *var_name, int i);
char	*getenv_hd(char *envp[], t_data *data, char *var_name);
char	*ft_itoa(int n);

int		heredoc_loop(t_data *data, t_node **infile_tmp, t_shell *parse, int ptr);
int		check_and_print_var_hd(char *str, t_data *data, int out_fd, int size);
int		cpvhd_specific_cases(t_data *data, char *str, int i, int output_fd);
int		print_var_hd(t_data *data, int var_size, char *var, int output_fd);
int		heredoc_exec(t_data *data, t_node **infile_tmp, t_shell *parse);
int		print_var_util(t_data *data, char *str, int i, int output_fd);
int		check_var_exists(int j, t_data *data, int output_fd);
int		backslash_check(t_data *data, char *str, int i);
int		check_delimiter(char *str, char *delimiter);
int		check_delimiter(char *str, char *delimiter);
int		check_special_char_second_loop(char c);
int		check_special_char(char c, int size);
int		check_for_append(t_node *infile_tmp);
int		check_eof(char *str, char *limiter);
int		var_exists_hd(t_data *data);

/* COMMAND UTILS */
void	first_command(char *envp[], t_data *data, t_node *node, t_shell *parse);
void	last_command(char *envp[], t_data *data, t_node *node, t_shell *parse);
void	command_exec(t_data *data, t_node *node, t_shell *parse, char *envp[]);
void	*commands(t_data *data, t_node *node, t_shell *parse, char *envp[]);
void	exec_cmd(char **tab, char **param, char *env[], t_data *data);
void	cmd_execution(t_data *data, char *envp[], int pipe_id);
void	first_cmd_execution(t_data *data, char *envp[]);
void	close_pipe_child_processes(t_data *data, int i);
void	last_cmd_execution(t_data *data, char *envp[]);
void	eof_handler(char *input, t_shell *minishell);
void	sigint_handler_in_process(int signum);
void	sigint_handler_main_loop(int signum);
void	close_pipe(t_data *data, int i);
void	*node_rotation(t_node *node);
void	free_param_tab(t_data *data);
void	sigtest(int signum);

char	*ft_strnstr(const char *haystack, const char *needle, size_t len);
char	**fill_param_tab(t_node *node, t_data *data, char **tab);
char	**get_path(char *env[], t_data *data, char **args);
char	**join_arg(char **tab, char **args);
char	**ft_split(const char *s, char c);
char	**free_tab(char **tab, int i);

int		check_inputfile_last_cmd(t_data *data, t_shell *parse);
int		check_outfile_last_cmd(t_data *data, t_shell *parse);
int		check_inputfile(t_data *data, t_shell *parse);
int		check_outfile(t_data *data, t_shell *parse);
int		ft_printf(const char *flags, ...);
int		**free_inttab(int **tab, int i);
int		pipe_creation(t_data *data);
int		check_sq_cmd(char *cmd);
int		fork_creation(int pid);
int		check_pipe(char *str);

#endif