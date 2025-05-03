/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arrayallocator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azahid <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 01:26:38 by azahid            #+#    #+#             */
/*   Updated: 2025/04/16 12:02:58 by azahid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_comm	*arrayallocator(char **arr, t_env *env)
{
	int		i;
	int		size;
	t_comm	*comms;

	i = 0;
	if (!arr)
		return (NULL);
	size = double_array_size(arr);
	if (size == -1)
		return (NULL);
	comms = (t_comm *)malloc((size + 1) * sizeof(t_comm));
	if (!comms)
		return (NULL);
	while (arr[i])
	{
		commandeparser(arr[i], &comms[i], env);
		if (DEBUG_MODE)
			print_t_comm(&comms[i]);
		if (DBG_ENV)
			print_t_env(&comms[i]);
		i++;
	}
	memset(&comms[i], 0, sizeof(t_comm));
	return (comms);
}
