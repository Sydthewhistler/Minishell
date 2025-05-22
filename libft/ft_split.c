/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprot <cprot@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 09:20:42 by cprot             #+#    #+#             */
/*   Updated: 2025/05/21 16:35:15 by cprot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(char const *s, char c)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != c && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (*s == c)
			in_word = 0;
		s++;
	}
	return (count);
}

static char	*get_next_word(char const *s, char c, int *i)
{
	int		len;
	char	*word;
	int		start;

	while (s[*i] && s[*i] == c)
		(*i)++;
	start = *i;
	len = 0;
	while (s[*i] && s[*i] != c)
	{
		len++;
		(*i)++;
	}
	word = malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	*i = start;
	len = 0;
	while (s[*i] && s[*i] != c)
		word[len++] = s[(*i)++];
	word[len] = '\0';
	return (word);
}

static void	free_split(char **tab, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

char	**ft_split(const char *s, char c)
{
	char	**tab;
	int		count;
	int		i;
	int		j;

	if (!s)
		return (NULL);
	count = count_words(s, c);
	tab = malloc(sizeof(char *) * (count + 1));
	if (!tab)
		return (NULL);
	i = 0;
	j = 0;
	while (j < count)
	{
		tab[j] = get_next_word(s, c, &i);
		if (!tab[j])
		{
			free_split(tab, j);
			return (NULL);
		}
		j++;
	}
	tab[count] = NULL;
	return (tab);
}
