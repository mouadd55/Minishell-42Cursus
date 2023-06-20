/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 20:32:43 by yonadry           #+#    #+#             */
/*   Updated: 2023/06/20 08:26:31 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	syntax_error(char *s2, char e)
{
	ft_putstr_fd("minishell: syntax error near unexpected token ", 2);
	if (s2)
		ft_putstr_fd(s2, 2);
	if (e)
		ft_putchar(e, 2);
	ft_putchar('\n', 2);
	return ;
}

int	search_for_pipe(t_vars *v)
{
	while (v->tmp1)
	{
		if (!ft_strcmp(v->tmp1->type, "PIPE"))
			return (1);
		v->tmp1 = v->tmp1->link;
	}
	return (0);
}

int	ft_isdigit(char *str)
{
	int	i;

	i = 0;
	if (str[i] && (str[i] == '-' || str[i] == '+'))
		if (!str[++i])
			return (0);
	while (str[i])
		if (!(str[i] >= '0' && str[i++] <= '9'))
			return (0);
	return (1);
}

void	initialize_variables(t_vars *v)
{
	v->vars = 0;
	v->flag = 0;
	v->var = NULL;
	v->val = NULL;
	v->temp2 = NULL;
	v->tmp3 = NULL;
}

void	split_string(t_vars *v, t_cmd *final_list, t_env **envr, int size)
{
	v->tmp1 = ft_split_input(v->str);
	lexer(&v->tmp1);
	if (size == 1)
		check_cmd(&v->tmp1, envr, final_list);
	ft_destroy_list(&v->tmp1);
	free(v->str);
	v->str = NULL;
}
