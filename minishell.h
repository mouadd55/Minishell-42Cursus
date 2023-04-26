/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonadry <yonadry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 13:25:55 by moudrib           #+#    #+#             */
/*   Updated: 2023/04/26 13:07:06 by yonadry          ###   ########.fr       */
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

/********************************* Libft utils ********************************/

int			ft_isalpha(int ch);
void		ft_putchar(char ch, int fd);
char		**ft_free_arr(char **str);
size_t		ft_strlen(const char *str);
char		*ft_strdup(const char *s1);
void		ft_putstr_fd(char *s, int fd);
char		*ft_strjoin(char *s1, char *s2);
char		**ft_split(char const *s, char c);
int			ft_strcmp(const char *s1, const char *s2);
char		*ft_substr(char const *s, unsigned int start, size_t len);
int			ft_strchr( char *str, int find);
int			ft_strrchr(char *str, int c);
/********************************* List utils *********************************/

t_list		*ft_lstnew(char *content);
t_list		*ft_lstlast(t_list *head);
void		*ft_destroy_list(t_list **head);
void		ft_lstadd_back(t_list **head, t_list *new);

/***************************** Parsing functions ******************************/

void		ft_first_check(char *input);
char		*ft_create_updated_input(char *input);
void		ft_fill_list(char *input, t_list **list);
void		syntax_error(char *s2, char e);

#endif