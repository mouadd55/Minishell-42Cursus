/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 13:25:55 by moudrib           #+#    #+#             */
/*   Updated: 2023/05/03 14:26:49 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include "string.h"
# include <readline/history.h>
# include <readline/readline.h>

typedef struct s_list
{
	char			*content;
	char			*key;
	char			*value;
	struct s_list	*link;
	struct s_list	*prev;
}	t_list;

typedef struct t_vars
{
	int		i;
	int		j;
	int		flag;
	int		count;
	int		d_quotes;
	char	**arr;
}	t_vars;

/********************************* Libft utils ********************************/

int			ft_isalpha(int ch);
char		**ft_free_arr(char **str);
size_t		ft_strlen(const char *str);
char		*ft_strdup(const char *s1);
void		ft_putchar(char ch, int fd);
int			ft_strrchr(char *str, int c);
void		ft_putstr_fd(char *s, int fd);
int			check_char(char *input, int c);
int			ft_strchr( char *str, int find);
char		*ft_strjoin(char *s1, char *s2);
char		**ft_split(char const *s, char *c);
int			ft_strcmp(const char *s1, const char *s2);
char		*ft_substr(char const *s, unsigned int start, size_t len);

/********************************* List utils *********************************/

t_list		*ft_lstnew(char *content);
t_list		*ft_lstlast(t_list *head);
void		*ft_destroy_list(t_list **head);
t_list		*ft_lstnew_env(char *key, char *value);
void		ft_lstadd_back(t_list **head, t_list *new);

/***************************** Parsing functions ******************************/

void		syntax_error(char *s2, char e);
int			ft_count_arguments(char *input);
int			ft_first_last_check(char *input);
t_list		*ft_split_environment(char **env);
void		env_parsing(char *input, t_list *env);
void		ft_fill_list(char *input, t_list **list);

int			check_single_quotes(char *input);
int			check_double_quotes(char *input);

#endif