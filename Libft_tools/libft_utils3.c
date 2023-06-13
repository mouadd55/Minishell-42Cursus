/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 04:27:23 by moudrib           #+#    #+#             */
/*   Updated: 2023/05/19 18:19:41 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strnstr(const char *str, const char *to_find, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if ((to_find == 0 || str == 0) && len == 0)
		return (NULL);
	if (*to_find == 0)
		return ((char *)str);
	while (str[i] && i < len)
	{
		while (to_find[j] == str[i + j] && str[i + j] && (i + j) < len)
			j++;
		if (!to_find[j])
			return ((char *)str + i);
		j = 0;
		i++;
	}
	return (NULL);
}

int	ft_count_words(char const *s, char *c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] && check_char(c, s[i]))
			i++;
		if (s[i])
			count++;
		while (s[i] && !check_char(c, s[i]))
			i++;
	}
	return (count);
}

static int	ft_lenword(char const *s, char *c)
{
	int	i;

	i = 0;
	while (s[i] && !check_char(c, s[i]))
		i++;
	return (i);
}

char	**ft_free_arr(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free (str[i]);
		i++;
	}
	free (str);
	return (0);
}

char	**ft_split(char const *s, char *c)
{
	int		i;
	char	**str;

	i = 0;
	if (!s)
		return (NULL);
	str = malloc((ft_count_words(s, c) + 1) * sizeof(char *));
	if (!str)
		return (NULL);
	while (*s)
	{
		while (check_char(c, *s))
			s++;
		if (*s)
		{
			str[i] = ft_substr(s, 0, ft_lenword(s, c));
			if (!str[i])
				return (ft_free_arr(str));
			i++;
		}
		while (!check_char(c, *s) && *s)
			s++;
	}
	str[i] = 0;
	return (str);
}
