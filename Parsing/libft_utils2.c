/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 20:01:56 by moudrib           #+#    #+#             */
/*   Updated: 2023/05/12 20:52:57 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

int	ft_isalpha(int ch)
{
	if ((ch >= 65 && ch <= 90) || (ch >= 97 && ch <= 122))
		return (1);
	return (0);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	j;
	char	*dest;

	i = 0;
	j = start;
	if (!s)
		return (NULL);
	if (len == 0 || (start >= ft_strlen(s)))
		return (ft_strdup(""));
	if (ft_strlen(s) < len)
		len = ft_strlen(s);
	if (ft_strlen(s + start) < len)
		len = ft_strlen(s + start);
	dest = malloc(len * sizeof(char) + 1);
	if (!dest)
		return (NULL);
	while (j < ft_strlen(s) && i < len)
		dest[i++] = s[j++];
	dest[len] = '\0';
	return (dest);
}

int	ft_strchr(char *str, int find)
{
	int	i;
	int	j;

	i = 0;
	j = ft_strlen(str);
	while (str[i])
	{
		if (str[i] == (char) find)
			return (i);
		i++;
	}
	return (0);
}

int	ft_strrchr(char *str, int c)
{
	int	srcl;

	srcl = ft_strlen(str);
	while (srcl >= 0)
	{
		if (str[srcl] == (char) c)
			return (srcl);
		srcl--;
	}
	return (0);
}
