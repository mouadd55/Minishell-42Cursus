/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonadry <yonadry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 15:31:06 by yonadry           #+#    #+#             */
/*   Updated: 2023/06/21 16:58:51 by yonadry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	switch_space(char *input, int x)
{
	t_vars	v;

	v.i = 0;
	while (input[v.i] && x)
	{
		if (input[v.i] == '\'' && ft_strchr(&input[v.i + 1], '\''))
		{
			while (input[++v.i] && input[v.i] != '\'')
				if (is_space(input[v.i]))
					input[v.i] *= -1;
		}
		else if (input[v.i] == '\"' && ft_strchr(&input[v.i + 1], '\"'))
		{
			while (input[++v.i] && input[v.i] != '\"')
				if (is_space(input[v.i]))
					input[v.i] *= -1;
		}
		v.i++;
	}
	while (input[v.i] && !x)
	{
		if (input[v.i] < 0)
			input[v.i] *= -1;
		v.i++;
	}
}

char	is_quote(char input)
{
	if (input == '\"' || input == '\'')
		return (input);
	return (0);
}

char	is_special(char c)
{
	if (check_char("()=+|><$?", c))
		return (c);
	return (0);
}

char	*is_redir(t_list *list)
{
	if (!ft_strcmp(list->type, "APPEND") || !ft_strcmp(list->type, "OUTFILE")
		|| !ft_strcmp(list->type, "INFILE"))
		return (list->content);
	return (0);
}

void	sig_hand(int sig)
{
	if (sig == SIGINT)
	{
		g_exit_status = -1;
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
	}
}
