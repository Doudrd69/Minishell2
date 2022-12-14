/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_outfile.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmonacho <wmonacho@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 19:34:37 by wmonacho          #+#    #+#             */
/*   Updated: 2022/11/17 10:52:39 by wmonacho         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

static void	include_outfile_list(t_node **tab_list, char *tmp)
{
	t_node	*list_cpy;

	add_back_file_list(tab_list, ft_dlstnew(tmp));
	list_cpy = *tab_list;
	while (list_cpy->next != NULL)
				list_cpy = list_cpy->next;
	list_cpy->type = 'C';
}

static int	main_loop_dl_fl(char *str, int i)
{
	while (str[i] != '\0' && str[i] != ' ' && str[i] != '<' && str[i] != '>')
	{
		if (str[i] == '\"' && str[i + 1] != '\0')
		{
			i++;
			while (str[i] != '\"' && str[i] != '\0')
				i++;
		}
		if (str[i] == '\'' && str[i + 1] != '\0')
		{
			i++;
			while (str[i] != '\'')
				i++;
		}
		i++;
	}
	return (i);
}

static void	delete_file_list(t_shell *minishell, t_node **list,
	char *cpy, char *str)
{
	int	i;
	int	j;

	i = minishell->mod;
	j = -1;
	while (++j < i)
		cpy[j] = str[j];
	i++;
	while (str[i] != '\0' && str[i] == ' ')
		i++;
	i = main_loop_dl_fl(str, i);
	while (str[i] != '\0' && str[i] == ' ')
		i++;
	while (str[i] != '\0')
		cpy[j++] = str[i++];
	cpy[j] = '\0';
	if (j == 0 || if_only_space(cpy) == 1)
	{
		free(cpy);
		cpy = NULL;
		unstack_list(minishell, list);
		return ;
	}
	include_dollar_list(minishell, list, cpy);
}

int	check_quote_outfile(t_shell *minishell, char *str, int len)
{
	int	i;
	int	dquote;

	i = 0;
	dquote = 0;
	minishell->quote = 0;
	while (i < len + 1 && str[i] != '\0')
	{
		if (str[i] == '\"' && minishell->quote != 1 && str[i - 1] != '\\')
			minishell->quote = 1;
		else if (str[i] == '\"' && minishell->quote != 0 && str[i - 1] != '\\')
			minishell->quote = 0;
		if (str[i] == '\'' && dquote != 1 && str[i - 1] != '\\')
			dquote = 1;
		else if (str[i] == '\'' && dquote != 0 && str[i - 1] != '\\')
			dquote = 0;
		if (str[i] == '>' && str[i + 1] != '>'
			&& (minishell->quote == 1 || dquote == 1))
			return (0);
		if (str[i] == '>' && str[i + 1] != '>'
			&& (minishell->quote == 0 || dquote == 0))
			return (1);
		i++;
	}
	return (1);
}

int	search_outfile(t_shell *minishell, char *str,
	t_node **tab_outfile, t_node **list)
{
	int		i;
	char	*tmp;
	char	*cpy;
	int		space;

	minishell->file_search = 0;
	i = minishell->mod;
	space = 0;
	if (check_syntax_outfile(minishell, str, i) == 0)
		return (0);
	while (str[++i] != '\0' && str[i] == ' ')
		space++;
	main_loop_search_outfile(minishell, str, i);
	search_outfile_malloc(minishell, str, &tmp, &cpy);
	tmp = cmd_cpy(tmp, str + (minishell->mod) + 1 + space,
			minishell->file_search + 1);
	include_outfile_list(tab_outfile, tmp);
	delete_file_list(minishell, list, cpy, str);
	minishell->mod = -1;
	return (1);
}
