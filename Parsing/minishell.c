/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 13:31:57 by moudrib           #+#    #+#             */
/*   Updated: 2023/05/27 22:51:26 by moudrib          ###   ########.fr       */
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

void	final(t_command *list)
{
	int			i;
	t_command	*tmp;

	tmp = list;
	printf("\n\e[1m\e[93m-----------------------------------------------------------------------\n");
	printf("|                            Final List                               |\n");
	printf("-----------------------------------------------------------------------\n");
	while (tmp)
	{
		i = 0;
		printf("|    ---------------------------------------                          |\n");
		printf("|    |               Command               |                          |\n");
		printf("|    ---------------------------------------                          |\n");
		while (tmp->cmd[i])
		{
			printf("|    |%37s|                          |\n", tmp->cmd[i]);
			i++;
		}
		printf("|    ---------------------------------------                          |\n");
		printf("|%29s %d                                      |\n", "fd_in:", tmp->fd_in);
		printf("|%30s %d                                     |\n", "fd_out:", tmp->fd_out);
		tmp = tmp->link;
	}
	printf("-----------------------------------------------------------------------\x1B[0m\n\n");
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

int	open_file(char *file_name, char *type)
{
	int fd = 1;

	if (!ft_strcmp(type, ">"))
	    fd = open(file_name, O_CREAT | O_RDWR | O_TRUNC, 0777);
	if (!ft_strcmp(type, ">>"))
	    fd = open(file_name, O_CREAT | O_RDWR | O_APPEND, 0777);
	if (fd == -1)
	{
		ft_printf_fd("Minishell: : Permission denied\n", 2, file_name);
		return (-1);
	}
	return (fd);
}

char *is_redir(t_list *list)
{
	if (!ft_strcmp(list->type, "APPEND")
		|| !ft_strcmp(list->type, "OUTFILE"))
		return (list->content);
	return (0);
}

int handle_file(t_list *list, char *file_name, char *type)
{
	int fd;
	
	fd = 1;
	while (list && !is_redir(list))
	{
		if (!ft_strcmp("VAR", list->type))
		{
			ft_printf_fd("Minishell: %s: ambiguous redirect", 2,list->content);
			return(-1);
		}
		if (!ft_strcmp("FILE", list->type))
			file_name = ft_strjoin(file_name, list->content);
		if (list->type[0] == 's' || !list->link
			||(list->link && is_special(list->link->content[0])))
		{
			fd = open_file(file_name, type);
			if (fd == -1)
				return (-1);
		}
		list = list->link;
	}
	return (fd);
}

int open_files(t_list *list)
{
	char *file_name;
	char *type;
	int fd;

	fd = 1;
	while (list)
	{
		if (is_redir(list))
		{
			file_name = NULL;
			type = ft_strdup(is_redir(list));
			if (list->link && !ft_strcmp(list->link->type, "space"))
				list = list->link->link;
			else if (list->link)
				list = list->link;
			fd = handle_file(list, file_name, type);
			if (fd == -1)
				return (-1);
		}
		if (!list)
			break;
		list = list->link;
	}
	return (fd);
}

void	minihell(char *input, t_env **envr, t_list **lst)
{
	int fd;
	t_command	*final_list;

	if (check_syntax(*lst))
		return ;
	lexer(lst);
	final_list = NULL;
	*envr = ft_builtins(input, envr);
	if (lst)
	{
		expand_var(lst, *envr);
		fd = open_files(*lst);
		check_cmd(lst, envr, input, fd);
		create_final_list(*lst, &final_list);
		final(final_list);
		// open_files(*lst);
		// ft(*lst);
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
