/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amabbadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 02:06:42 by amabbadi          #+#    #+#             */
/*   Updated: 2025/04/14 02:06:45 by amabbadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	unset(t_env **env, char *var)
{
	t_env *curr;
	t_env *prev;

	if (!env || !var || !*var)
		return (0);

	curr = *env;
	prev = NULL;

	while (curr)
	{
		if (curr->key && ft_strcmp(curr->key, var) == 0 && (!ft_strcmp(var,
					"SHELL") || (!ft_strcmp(var, "_"))))
			// delete shell check after it does not delete in bash just sayiinn
			break ;
		if (curr->key && ft_strcmp(curr->key, var) == 0)
		{
			if (prev)
				prev->next = curr->next;
			else
				*env = curr->next;
			free(curr->key);
			free(curr->value);
			free(curr);
			return (0);
		}
		prev = curr;
		curr = curr->next;
	}
	return (0);
}