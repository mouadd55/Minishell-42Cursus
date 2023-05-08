/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonadry <yonadry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 20:32:43 by yonadry           #+#    #+#             */
/*   Updated: 2023/05/07 19:58:44 by yonadry          ###   ########.fr       */
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