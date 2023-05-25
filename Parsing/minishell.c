/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 13:31:57 by moudrib           #+#    #+#             */
/*   Updated: 2023/05/25 22:27:29 by moudrib          ###   ########.fr       */
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

t_env	*ft_builtins(char *input, t_env **env)
{
	env_parsing(input, *env);
	return (*env);
}

t_env	*ft_copy_env_list(t_env *env)
{
	t_env	*copy;

	copy = NULL;
	while (env)
	{
		if (env->value)
			ft_lstadd_back_env(&copy,
				ft_lstnew_env(ft_strdup(env->key), ft_strdup(env->value)));
		else
			ft_lstadd_back_env(&copy, ft_lstnew_env(ft_strdup(env->key), NULL));
		env = env->link;
	}
	return (copy);
}

int	open_file(t_list *list, char *file_name, char *type)
{
	int fd = 1;

	if (!ft_strcmp(type, "OUTFILE"))
	    fd = open(file_name, O_CREAT | O_RDWR | O_TRUNC, 0777);
	if (!ft_strcmp(type, "APPEND"))
	    fd = open(file_name, O_CREAT | O_RDWR | O_APPEND, 0777);
	if (fd == -1)
	{
		ft_putstr_fd("Minishell: ", 2);
		ft_putstr_fd(file_name, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		return (-1);
	}
	return (fd);
}

int handle_file(t_list *list, char *file_name, char *i_file_name, char *type)
{
	int fd;
	
	while (list)
	{
		if (!ft_strcmp(list->type, "APPEND"))
			return (1);
		if (list->type[0] == 'F')
			file_name = ft_strjoin(file_name, list->content);
		else if (list->type[0] == 'C')
			i_file_name = ft_strjoin(i_file_name, list->content);
		if (list->type[0] == 's' || !list->link)
		{
			fd = open_file(list, file_name, type);
			if (fd == -1)
				return (1);
			if (i_file_name)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(i_file_name, 2);
				ft_putstr_fd(": No such file or directory\n", 2);
				free(i_file_name);
				i_file_name = NULL;
			}
		}
		list = list->link;
	}
}

void open_files(t_list *list)
{
	char *file_name;
	char *i_file_name;
	char *type;

	i_file_name = NULL;
	while (list)
	{
		if (!ft_strcmp(list->type, "APPEND"))
		{
			file_name = NULL;
			type = ft_strdup(list->type);
			if (list->link &&  !ft_strcmp(list->link->type, "space"))
				list = list->link->link;
			else if (list->link)
				list = list->link;
			handle_file(list, file_name, i_file_name, type);
		}
		if (!list)
			break;
		list = list->link;
	}
}

void	minihell(char *input, t_env **envr, t_list **lst)
{
	if (check_syntax(*lst))
		return ;
	lexer(lst);
	*envr = ft_builtins(input, envr);
	if (lst)
	{
		expand_var(lst, *envr);
		check_cmd(lst, envr, input);
		open_files(*lst);
		ft(*lst);
	}
}

int	main(int ac, char **av, char **env)
{
	char	*input;
	t_env	*envr;
	t_list	*lst;

	(void)av;
	if (ac != 1)
		return (0);
	envr = NULL;
	envr = ft_split_environment(env);
	while (1)
	{
		input = readline("➜  Minishell ");
		if (!input)
			break ;
		if (ft_strlen(input))
		{
			add_history(input);
			lst = ft_split_input(input);
			if (lst)
				minihell(input, &envr, &lst);
		}
		free(input);
	}
	return (0);
}
