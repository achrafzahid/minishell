/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azahid <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 00:46:07 by azahid            #+#    #+#             */
/*   Updated: 2025/04/14 05:27:50 by azahid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	counter(char *str)
{
	int		i;
	int		count;
	char	quote;

	i = 0;
	count = 1;
	while (str[i])
	{
		if (isquote(str[i]))
		{
			quote = str[i++];
			while (str[i] && str[i] != quote)
				i++;
			if (str[i])
				i++;
		}
		else if (str[i] == '|')
		{
			count++;
			i++;
		}
		else
			i++;
	}
	return (count);
}

char	**pipe_split(char *str)
{
	int		i, start, part;
	int		total;
	char	**result;
	char	quote;
	int		len;

	i = 0, start = 0, part = 0;
	total = counter(str);
	result = malloc((total + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	while (str[i])
	{
		if (isquote(str[i]))
		{
			quote = str[i++];
			while (str[i] && str[i] != quote)
				i++;
			if (str[i])
				i++;
		}
		else if (str[i] == '|')
		{
			len = i - start;
			result[part++] = ft_strndup(str + start, len);
			i++;
			start = i;
		}
		else
		{
			i++;
		}
	}
	len = i - start;
	result[part++] = ft_strndup(str + start, len);
	result[part] = NULL;
	return (result);
}
