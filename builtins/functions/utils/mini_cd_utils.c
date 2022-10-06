#include "../../../includes/minishell.h"

int find_pwd(t_mini_data *data, char *var)
{
	int	i;

	i = 0;
	while (i < data->envp_size)
	{
		if (ft_strnstr(data->env[i], var, ft_strlen(var)))
		{
			if (check_var(data->env[i], var))
				return (i);
		}
		i++;
	}
	return (i);
}

int	update_pwd(t_mini_data *data)
{
	int		position;
	int		size;
	char	*str;
	char	*cwd;

	position = find_pwd(data, "PWD");
	if (position == data->envp_size)
		return (1);
	cwd = getcwd(data->buff, BUF_SIZE);
	data->oldpwd = cwd;
	size = ft_strlen(cwd);//taille de VALUE
	str = malloc(sizeof(char) * size + 5);//5 pour PWD= et le \0
	if (!str)
		return (1);
	ft_strlcpy(str, "PWD=", 5, 0);
	ft_strlcpy(&str[4], cwd, ft_strlen(cwd), 1);
	ft_strlcpy(data->env[position], str, ft_strlen(str), 1);
	free(str);
	return (0);
}

int	update_old_pwd(t_mini_data *data)
{
	int		position;
	int		size;
	char	*str;

	position = find_pwd(data, "OLDPWD");
	if (position == data->envp_size)
		return (1);
	size = ft_strlen(data->oldpwd);
	str = malloc(sizeof(char) * size + 8);//8 pour OLDPWD= et le \0
	if (!str)
		return (1);
	ft_strlcpy(str, "OLDPWD=", 7, 0);
	ft_strlcpy(&str[6], data->oldpwd, ft_strlen(data->oldpwd), 1);
	ft_strlcpy();
}

//quand on lance le shell --> OLDPWD = NULL et PWD = current
	//quand on va faire cd, on update PWD avec le current et OLDPWD est créé
	//et vaut current - 1