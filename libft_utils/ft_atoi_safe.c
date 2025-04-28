/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_safe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amabbadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 20:38:04 by amabbadi          #+#    #+#             */
/*   Updated: 2025/04/28 20:38:05 by amabbadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

long long ft_atoi_safe(char *str, int *error)
{
	int				sign;
	unsigned long long	num;
	int				digit;

	sign = 1;
	num = 0;
	*error = 0;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	if (!*str)
	{
		*error = 1;
		return (0);
	}
	if (sign == -1 && ft_strcmp(str, "9223372036854775808") == 0)
	{
		return (LLONG_MIN);
	}

	while (*str)
	{
		if (*str < '0' || *str > '9')
		{
			*error = 1;
			return (0);
		}
		digit = *str - '0';
		if (num > (unsigned long long)(LLONG_MAX - digit) / 10)
		{
			*error = 1;
			return (0);
		}
		num = num * 10 + digit;
		str++;
	}
	if (sign == 1 && num > LLONG_MAX)
	{
		*error = 1;
		return (0);
	}
	if (sign == -1 && num > (unsigned long long)LLONG_MAX + 1)
	{
		*error = 1;
		return (0);
	}
	return ((sign * num));
}
