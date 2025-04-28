/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_com_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azahid <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 20:18:10 by azahid            #+#    #+#             */
/*   Updated: 2025/04/22 21:28:54 by azahid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// echo > "outer liner bitcher" by
int	count_checker(char *s)
{
	int		i;
	int		count;
	char	quote;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] && ft_isspace(s[i]))
			i++;
		if (s[i] && !ft_isspace(s[i]))
		{
			if (is_redirection(s[i]))
			{
				i++;
				if (isquote(s[i]))
				{
					while (s[i] && ft_isspace(s[i]))
						i++;
					while (s[i] && isquote(s[i]))
					{
						quote = s[i++];
						while (s[i] && s[i] != quote)
							i++;
						if (s[i])
							i++;
					}
				}
				else
				{
					while (s[i] && ft_isspace(s[i]))
						i++;
					while (s[i] && !ft_isspace(s[i]) && !is_redirection(s[i]))
						i++;
				}
			} // echo ""ls""
			else if (isquote(s[i]))
			{
				count++;
				while (s[i] && (isquote(s[i]) || !ft_isspace(s[i])))
				{
					quote = s[i++];
					while (s[i] && s[i] != quote)
						i++;
					if (s[i])
						i++;
				}
			}
			else
			{
				count++;
				while (s[i] && !ft_isspace(s[i]) && !is_redirection(s[i]))
				{
					if (isquote(s[i]))
					{
						quote = s[i++];
						while (s[i] && s[i] != quote)
							i++;
						if (s[i])
							i++;
					}
					else
						i++;
				}
			}
		}
	}
	return (count);
}

t_chars	*p_com_split(char *str, t_comm *com)
{
	int		count;
	char	**res;
	int		i;
	t_chars	*s;

	i = 0;
	s = NULL;
	if (!str)
		return (NULL);
	count = count_checker(str);
	if (count == 0)
	{
		com->flag += 1;
		return (NULL);
	}
	res = (char **)malloc((count + 1) * sizeof(char *));
	if (!res)
		return (NULL);
	res = allocate(res, str, count);
	if (!res)
		return (NULL);
	while (res[i])
	{
		push_to_list(&s, ft_strdup(res[i]), -1);
		i++;
	}
	free2d(res);
	return (s);
}
