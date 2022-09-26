#include "../../cmd_include/pipex_bonus.h"

int	check_outfile_last_cmd(t_data *data)
{
	if (data->exec.last_cmd_outfile_check == 1)
	{
		if (data->exec.append_check == 1)
			data->output_fd = open(data->exec.outfile_fd, O_WRONLY | O_CREAT, 0666);
		else
		{
			data->output_fd = open(data->exec.outfile_fd, O_WRONLY | O_TRUNC
					| O_CREAT, 0666);
		}
		if (data->output_fd < 0)
		{
			ft_printf("Error : can't open file : %s\n");//afficher le bon fichier
			return (0);
		}
		if (dup2(data->output_fd, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			return (0);
		}
		return (1);
	}
	data->output_fd = STDOUT_FILENO;
	return (0);
}
