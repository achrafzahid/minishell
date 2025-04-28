/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azahid <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 03:29:37 by azahid            #+#    #+#             */
/*   Updated: 2025/03/26 03:29:49 by azahid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strnstr(const char *str, const char *tofind, size_t len)
{
	int	i;
	int	j;
	int	holder;

	i = 0;
	if (*tofind == '\0')
		return ((char *)str);
	if (tofind == str)
		return ((char *)str);
	while (str[i] && len)
	{
		j = 0;
		holder = len;
		while (tofind[j] && str[i] && holder-- && str[i + j] == tofind[j])
			j++;
		if (!tofind[j])
			return ((char *)str + i);
		i++;
		len--;
	}
	return (NULL);
}
