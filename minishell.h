/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 13:25:55 by moudrib           #+#    #+#             */
/*   Updated: 2023/05/12 16:29:48 by moudrib          ###   ########.fr       */
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
	char			*type;
	struct s_list	*link;
	struct s_list	*prev;
}	t_list;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*link;
	struct s_env	*prev;
}	t_env;

typedef struct t_vars
{
	int		i;
	int		j;
	int		flag;
	int		count;
	int		d_quotes;
	char	**arr;
	int		start;
	int		end;
	char	*str;
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
char		*ft_strtrim(char *s1, char *set);

/********************************* List utils *********************************/

t_list		*ft_lstnew(char *content);
t_list		*ft_lstlast(t_list *head);
t_env		*ft_lstlast_env(t_env *head);
void		*ft_destroy_list(t_list **head);
void		*ft_destroy_list_env(t_env **head);
t_env		*ft_lstnew_env(char *key, char *value);
void		ft_lstadd_back(t_list **head, t_list *new);
void		ft_lstadd_back_env(t_env **head, t_env *new);
int			ft_lstsize(t_list *lst);
/***************************** Parsing functions ******************************/

void		syntax_error(char *s2, char e);
t_env		*ft_split_environment(char **env);
void		env_parsing(char *input, t_env *env);
t_list		*ft_split_input(char *input);
int			check_syntax(t_list *lst);
int			export_parsing(char *input);
void		lexer(t_list **list);
void		delete_node(t_list **list, int position);
int			check_before_value(t_list **list);

// int			ft_count_arguments(char *input);
// int			ft_first_last_check(char *input);
// void		ft_fill_list(char *input, t_list **list);
// int			check_single_quotes(char *input);
// int			check_double_quotes(char *input);

#endif