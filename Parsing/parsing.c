/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 15:52:52 by moudrib           #+#    #+#             */
/*   Updated: 2023/04/29 18:01:06 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_char(char *input, int c)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == (char)c)
			return (1);
		i++;
	}
	return (0);
}

size_t	ft_count_char(char *input, char c)
{
	size_t	i;
	size_t	count;

	i = -1;
	count = 0;
	while (input[++i])
	{
		if (input[i] == c)
			count++;
	}
	return (count);
}

int	ft_first_middle_check(char *input)
{
	size_t	i;

	i = 0;
	while (i < ft_strlen(input))
	{
		if ((input[i] == '>' && input[i + 1] == '<') || (input[i] == '<'
				&& input[i + 1] == '>'))
		{
			syntax_error(NULL, input[i]);
			return (1);
		}
		else if (((check_char("<>", input[i]) && check_char("<>", input[i + 1]))
				|| (input[i] == '|' && input[i + 1] == '|')) && (!input[i
					+ 2] || ft_count_char(&input[i + 2],
					' ') == ft_strlen(&input[i + 2])))
			return (syntax_error(&input[i], 0), 1);
		else if (input[i] == '|' && (input[i + 1] == '|'
				|| ft_count_char(&input[i + 1], ' ') == ft_strlen(&input[i
						+ 1])))
			return (syntax_error(NULL, '|'), 1);
		else if (input[i] == '(' || input[i] == ')')
			return (syntax_error(NULL, input[i]), 1);
		i++;
	}
	return (0);
}

int	ft_check_double_specials(char *input)
{
	int		i;
	char	*tmp;

	i = -1;
	tmp = NULL;
	if (check_char("><", input[0]) || ft_strchr(input, '>') || ft_strchr(input,
			'<') || ft_strchr(input, '|'))
	{
		while (input[++i])
		{
			if (check_char("|><", input[i]))
			{
				if (check_char("><", input[i + 1]))
					tmp = &input[i] + 2;
				else
					tmp = &input[i] + 1;
				break ;
			}
		}
		i = 0;
		while (tmp[i] && !ft_isalpha(tmp[i]))
		{
			if (check_char("><|", tmp[i]))
				return (syntax_error(NULL, tmp[i]), 1);
			i++;
		}
		if (ft_strchr(&tmp[i], '>') || ft_strchr(&tmp[i], '<')
			|| ft_strchr(&tmp[i], '|'))
			ft_check_double_specials(&tmp[i]);
	}
	return (0);
}

int	check_double_quotes(char *input)
{
	int	i;
	int	count;

	i = 0;
	count = 1;
	if (ft_strchr(input, '\"') || input[0] == '\"')
	{
		i = ft_strchr(input, '\"');
		while (input[i++])
		{
			if (input[i] == '\"')
			{
				count++;
				break ;
			}
		}
		if (count % 2 != 0)
		{
			syntax_error(NULL, '\"');
			return (1);
		}
		else
			check_double_quotes(&input[i + 1]);
	}
	return (0);
}

int	check_single_quotes(char *input)
{
	int	i;
	int	flag;
	int	count;
	int	d_quotes;

	i = 0;
	flag = 0;
	count = 0;
	d_quotes = 0;
	while (input[i])
	{
		if (input[i] == '\"' && (d_quotes % 2 == 0))
		{
			flag = 1;
			d_quotes++;	
		}
		else if (input[i] == '\"' && (d_quotes % 2 != 0))
		{
			flag = 0;
			d_quotes++;	
		}
		if (input[i] == '\\' && input[i + 1] && input[i + 1] == '\'')
			i += 2;
		else if (input[i] == '\'')
		{
			if (flag == 0)
				count++;
			i++;
		}
		else
			i++;
	}
	if (count % 2)
		return (syntax_error(NULL, '\''), 1);
	return (0);
}

int	ft_first_last_check(char *input)
{
	size_t	i;

	i = -1;
	if (check_char("()|", input[0]) || (check_char("<>", input[0]) && (!input[1]
				|| ft_count_char(&input[0], ' ') == ft_strlen(&input[1]))))
		return (syntax_error(NULL, input[0]), 0);
	else if (ft_first_middle_check(input))
		return (0);
	else if (check_double_quotes(input))
		return (0);
	else if (check_single_quotes(input))
		return (0);
	else
		ft_check_double_specials(input);
	return (1);
}
