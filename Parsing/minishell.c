/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonadry <yonadry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 13:31:57 by moudrib           #+#    #+#             */
/*   Updated: 2023/05/14 16:25:23 by yonadry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft(t_list *stack)
{
	t_list	*tmp;

	tmp = stack;
	printf("\n                 \e[1m\e[93mList");
	printf("\n---------------------------------------\n");
	printf("|   Token          |           Type   |\n");
	printf("---------------------------------------\n");
	while (tmp)
	{
		printf("|%18s|%18s|\n", tmp->content, tmp->type);
		tmp = tmp->link;
	}
	printf("---------------------------------------\x1B[0m\n\n");
}

t_env	*ft_builtins(char *input, char **env, t_list *list)
{
	t_env	*envr;

	envr = ft_split_environment(env);
	// env_parsing(input, envr);
	// export_parsing(input);

	return (envr);
}
void index_list(t_list **list)
{
	t_list *tmp;
	int		count;

	tmp = *list;
	count = 0;
	while (tmp)
	{
		tmp->pos = count;
		count++;
		tmp = tmp->link;
	}
}

t_list *del_node(t_list **list, t_list *del_node)
{
	t_list *tmp;
	t_list *tmp1 = NULL;

	tmp = *list;
	if (del_node == tmp)
	{
		tmp = tmp->link;
		free(tmp->prev);
		tmp->prev = NULL;
		return(tmp);
	}
	else if (del_node == ft_lstlast(tmp))
	{
		tmp1 = ft_lstlast(tmp);
		tmp1 = tmp1->prev;
		free(tmp1->link);
		tmp1->link = NULL;
	}
	else
	{
		while (tmp)
		{
			if (tmp == del_node)
			{
				tmp1 = tmp->prev;
				tmp1->link = tmp->link;
				tmp->link->prev = tmp1;
				free(tmp);
				tmp = tmp1;
			}
			tmp = tmp->link;
		}
	}
	return (tmp1);
}
void remove_quotes(t_list **list)
{
	t_list *temp;

	temp = *list;
	while (temp)
	{
		if (!ft_strcmp("DOUBLE_Q", temp->type) || !ft_strcmp("SINGLE_Q", temp->type))
		{
			if (ft_strlen(temp->content) == 2)
				temp->content = ft_strdup("");
			else
				temp->content = ft_substr(temp->content, 1, ft_strlen(temp->content)-2);
		}
		temp = temp->link;
	}
	
}

void expand_in_quotes(t_list **list, t_env *envr)
{
	t_list *temp;
	t_vars v;

	v.i = 0;
	temp = *list;
	while (temp)
	{
		if (check_char(temp->content, '$') && !ft_strcmp("DOUBLE_Q", temp->type))
		{
			while (temp->content[v.i])
			{
				while (temp->content[v.i] != '$')
					v.i++;
				v.str = ft_substr(temp->content, 0, v.i);
				while (ft_isalpha(temp->content[v.i])
					|| check_char("0123456789", temp->content[v.i]))
					v.i++;
			}
		}
		temp = temp->link;
	}
	
}

void expand_var(t_list **list, t_env *envr)
{
	t_list *tmp = *list;
	// t_list *save_node;
	t_env *tmp1 = envr;
	t_vars v;
	v.i = 0;
	while (tmp)
	{
		if (tmp && tmp->content[0] == '$' && tmp->link)
		{
			tmp = tmp->link;
			while (ft_isalpha(tmp->content[v.i]) ||
				check_char("0123456789", tmp->content[v.i]))
				v.i++;
			v.str = ft_substr(tmp->content, 0, v.i);
			tmp1 = envr;
			while (tmp1)
			{
				if (tmp1 && !ft_strcmp(tmp1->key, v.str))
				{
					tmp->prev->content = ft_strdup(tmp1->value);
					tmp->prev->content = ft_strjoin(tmp->prev->content ,ft_strdup(&tmp->content[v.i]));
					tmp =  del_node(list, tmp);
					break;
				}
				tmp1 = tmp1->link;
			}
		}
		tmp = tmp->link;
	}
	remove_quotes(list);
	expand_in_quotes(list, envr);
}

t_list	*minihell(char *input)
{
	t_list	*lst;

	lst = ft_split_input(input);
	if (check_syntax(lst))
		return (0);
	lexer(&lst);
	// if (ft_strlen(input) && (export_parsing(input) || check_before_value(&lst)))
	// 	return (0);
	return (lst);
}

int	main(int ac, char **av, char **env)
{
	char	*input;
	t_env	*envr;
	t_list	*lst;

	(void)av;
	(void)env;
	if (ac != 1)
		return (0);
	while (1)
	{
		input = readline("➜  Minishell ");
		if (!input)
			break ;
		if (ft_strlen(input))
			add_history(input);
		lst = minihell(input);
		if (lst)
		{
			envr = ft_builtins(input, env, lst);
			lexer(&lst);
			expand_var(&lst, envr);
			ft(lst);
			// ft_destroy_list(&lst);
		}
		// ft_destroy_list_env(&envr);
		free(input);
	}
	return (0);
}
