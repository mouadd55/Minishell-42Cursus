/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 13:25:55 by moudrib           #+#    #+#             */
/*   Updated: 2023/03/25 02:12:58 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <readline/history.h>
# include <readline/readline.h>

typedef struct s_list
{
	char			*content;
	struct s_list	*link;
}	t_list;

/************************* Libft utils *************************/

void		ft_putchar(char ch);
void		ft_putstr(char *str);
size_t		ft_strlen(const char *str);
char		*ft_strdup(const char *s1);
char		*ft_strjoin(char *s1, char *s2);
int			ft_strcmp(const char *s1, const char *s2);

/**************************** List utils ***************************/

t_list		*ft_lstnew(int data, char *content);
t_list		*ft_lstlast(t_list *head);
void		*ft_destroy_list(t_list **head);
void		ft_lstadd_back(t_list **head, t_list *new);

#endif