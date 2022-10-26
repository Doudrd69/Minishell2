#include "../parsing.h"

static void	init_var_redirection(t_shell *minishell, int size)
{
	minishell->tab_infile = (t_node **)malloc(sizeof(t_node *) * (size + 1));
	minishell->tab_outfile = (t_node **)malloc(sizeof(t_node *) * (size + 1));
}

static void	classic_infile(char *str, int tab, t_shell *minishell)
{
	int	i;

	i = minishell->mod;
	if (str[i] == '<' && str[i + 1] != '\0' && str[i + 1] != '<')
	{
		search_infile(minishell, str, tab);
		printf("classic infile\n");
	}
}

static void	append_infile(char *str, int tab, t_shell *minishell)
{
	int	i;

	i = minishell->mod;
	tab += 0;
	if (str[i] == '<' && str[i + 1] != '\0' && str[i + 1] == '<')
	{
		i++;
		printf("append infile\n");
	}
}

static void	classic_outfile(char *str, int tab, t_shell *minishell)
{
	int	i;

	i = minishell->mod;
	tab += 0;
	if (str[i] == '>' && str[i + 1] != '\0' && str[i + 1] != '>')
		printf("classic outfile\n");
}

static void	append_outfile(char *str, int tab, t_shell *minishell)
{
	int	i;

	i = minishell->mod;
	tab += 0;
	if (str[i] == '>' && str[i + 1] != '\0' && str[i + 1] == '>')
	{
		i++;
		printf("append outfile\n");
	}
}

void	parse_redirections(t_shell *minishell)
{
	int		size;
	t_node	*list_cpy;
	char	*str;
	int		j;

	j = 0;
	size = minishell->nbr_pipe + 1;
	init_var_redirection(minishell, size);
	list_cpy = minishell->head;
	while (list_cpy && list_cpy != NULL)
	{
		if (list_cpy->type == 'P')
			j++;
		str = (char *)(list_cpy->content);
		minishell->mod = -1;
		while (str[++(minishell->mod)] != '\0')
		{
			classic_infile(str, j, minishell);
			append_infile(str, j, minishell);
			classic_outfile(str, j, minishell);
			append_outfile(str, j, minishell);
		}
		list_cpy = list_cpy->next;
	}
}
