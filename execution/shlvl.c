/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shlvl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amabbadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 15:44:40 by amabbadi          #+#    #+#             */
/*   Updated: 2025/04/28 15:44:41 by amabbadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	increment_shlvl(t_env *env)
{
	int		shlvl;
	char	*tmp;

	while (env)
	{
		if (env->key && !strcmp(env->key, "SHLVL"))
		{
			if (!env->value)
				return ;
			shlvl = atoi(env->value);
			shlvl++;
			tmp = ft_itoa(shlvl);
			if (!tmp)
				return ;
			free(env->value);
			env->value = tmp;
			return ;
		}
		env = env->next;
	}
}
