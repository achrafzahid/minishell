/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hollyparser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azahid <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 02:09:12 by azahid            #+#    #+#             */
/*   Updated: 2025/04/19 08:30:50 by azahid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	expand_variable(char *src, t_env *env, char *dest, int *si)
{
	char	var[256] = {0};
	int		var_len;
	char	*val;
	int		len;

	// int		i;
	var_len = 0;
	len = 0;
	if (!src || !dest || !env)
		return (0);
	// Parse variable name
	get_var_name(src, var, 256, &var_len);
	if (var_len == 0) // No valid variable
	{
		*si += 1; // Skip $
		return (0);
	}
	if (var[0] == '?' && var[1] == '\0')
	{
		char status_str[12]; // enough for intmax
		len = snprintf(status_str, sizeof(status_str), "%d", env->exit_status);
		for (int j = 0; j < len; j++)
			dest[j] = status_str[j];
		*si += 2;
		return (len);
	}
	val = get_env_value(var, env);
	if (val)
	{
		while (val[len])
		{
			dest[len] = val[len];
			len++;
		}
	}
	*si += var_len + 1;
	return (len);
}

char	**parser(char *str, t_env *env, int flag, int type)
{
	int		countw;
	char	*res;
	char	**result;
	int		f;

	f = 0;
	flag++;
	if (!str)
		return (NULL);
	if (str[0] == '\0')
	{
		res = ft_strdup("");
		if (!res && env)
			env->exit_status = 1;
		return (ft_split(res, 0));
	}
	countw = cw(str, env, type);
	if (countw == -1)
		return (NULL);
	res = malloc(sizeof(char) * (countw + 1));
	if (!res)
	{
		if (env)
			env->exit_status = 1;
		printf("bash: cannot allocate memory\n");
		return (NULL);
	}
	res = fill_word(res, str, env, type, &f);
	if (f)
		result = ft_split(res, ' ');
	else
		result = ft_split(res, 0);
	return (free(res), result);
}
