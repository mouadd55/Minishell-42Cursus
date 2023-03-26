/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 13:31:57 by moudrib           #+#    #+#             */
/*   Updated: 2023/03/26 17:37:45 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_duplicate_inputs_in_history(t_list **list)
{
	char	*input;
	char	*previous_input;
	(void)list;

	input = readline("➜  Minishell ");
	if (ft_strlen(input))
		add_history(input);
	while (1)
	{
		previous_input = input;
		input = readline("➜  Minishell ");
		if (!input)
			break ;
		if (ft_strlen(input) && ft_strcmp(input, previous_input))
			add_history(input);
	}
	return (input);
}

int	main(int ac, char **av)
{
	char	*input;
	t_list	*list;

	(void)av;
	if (ac != 1)
		return (0);
	input = ft_duplicate_inputs_in_history(&list);
	free(input);
	return (0);
}
 