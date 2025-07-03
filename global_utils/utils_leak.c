/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   leak_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amabbadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 02:07:00 by amabbadi          #+#    #+#             */
/*   Updated: 2025/04/16 19:39:35 by azahid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free1d(void *array)
{
	if (array != NULL)
		free(array);
}

void	free2d(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

void	free_chars(t_chars *list)
{
	t_chars *tmp;

	while (list)
	{
		tmp = list;
		list = list->next;
		if (tmp->str)
			free(tmp->str);
		free(tmp);
	}
}

void	free_ints_list(t_ints *head)
{
	t_ints *tmp;

	while (head)
	{
		tmp = head->next;
		free(head);
		head = tmp;
	}
}

void	free_commande(t_comm *com)
{
	t_chars *tmp;

	if (!com)
		return ;
	while (com->p_com)
	{
		tmp = com->p_com->next;
		if (com->p_com->str)
			free(com->p_com->str);
		free(com->p_com);
		com->p_com = tmp;
	}
	if (com->commande)
	{
		if (com->commande->str)
			free2d(com->commande->str);
		free(com->commande);
	}
	if (com->redirections)
		free_chars(com->redirections);
	if (com->heardoc)
		free_chars(com->heardoc);
}

void	free_all_commande(t_comm *comms, int size)
{
	int i;

	if (!comms)
		return ;
	i = 0;
	while (i < size)
	{
		free_commande(&comms[i]);
		i++;
	}
	free(comms);
}

void	free_env(t_env *head)
{
	t_env *tmp;

	while (head)
	{
		tmp = head->next;
		if (head->key)
			free(head->key);
		if (head->value)
			free(head->value);
		free(head);
		head = tmp;
	}
}
