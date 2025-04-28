
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commandeparser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azahid <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 03:09:11 by azahid            #+#    #+#             */
/*   Updated: 2025/04/21 21:37:30 by azahid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	parse_wildcards_list(t_comm *com)
{
	t_chars	*curr;
	t_chars	*prev;
	t_chars	*next;
	t_chars	*expanded;
	t_chars	*last;

	curr = com->p_com;
	prev = NULL;
	while (curr)
	{
		if (ft_strchr(curr->str, '*') || ft_strchr(curr->str, '?'))
		{
			next = curr->next;
			expanded = parse_wildcards(curr->str, com);
			if (!expanded)
			{
				free_chars(com->p_com);
				com->p_com = NULL;
				return (1);
			}
			last = expanded;
			while (last->next)
				last = last->next;
			if (prev)
				prev->next = expanded;
			else
				com->p_com = expanded;
			last->next = next;
			free(curr->str);
			free(curr);
			curr = next;
			prev = last;
		}
		else
		{
			prev = curr;
			curr = curr->next;
		}
	}
	return (0);
}

static int	parse_expand_vars(t_comm *com, t_env *env)
{
	t_chars	*curr;
	t_chars	*prev;
	char	**tmp;
	t_chars	*new_head;
	t_chars	*new_tail;
	t_chars	*node;
	t_chars	*nxt;

	curr = com->p_com;
	prev = NULL;
	while (curr)
	{
		if (curr->str[0] == '\"' || curr->str[0] == '\'')
			tmp = parser(curr->str, env, 0, 0);
		else
			tmp = parser(curr->str, env, 1, 0);
		if (!tmp)
		{
			free_chars(com->p_com);
			com->p_com = NULL;
			return (1);
		}
		new_head = NULL;
		new_tail = NULL;
		for (int j = 0; tmp[j]; j++)
		{
			node = malloc(sizeof(t_chars));
			if (!node)
			{
				free2d(tmp);
				free_chars(com->p_com);
				com->p_com = NULL;
				return (1);
			}
			node->str = ft_strdup(tmp[j]);
			node->type = curr->type;
			node->next = NULL;
			if (!node->str)
			{
				free(node);
				free2d(tmp);
				free_chars(com->p_com);
				com->p_com = NULL;
				return (1);
			}
			if (!new_head)
				new_head = new_tail = node;
			else
			{
				new_tail->next = node;
				new_tail = node;
			}
		}
		if (!prev)
			com->p_com = new_head;
		else
			prev->next = new_head;
		if (new_tail)
			new_tail->next = curr->next;
		else if (prev)
			prev->next = curr->next;
		nxt = curr->next;
		free(curr->str);
		free(curr);
		curr = nxt;
		prev = new_tail ? new_tail : prev;
		free2d(tmp);
	}
	return (0);
}

static int	parse_finalize(t_comm *com, char *arr)
{
	char	*new_str;

	if (!com)
		return (1);
	if (!com->p_com)
	{
		if (com->env)
			com->env->exit_status = 0;
		return (0);
	}
	if (check_builtin(com))
	{
		new_str = createargs(com);
		if (!new_str)
		{
			free_chars(com->p_com);
			com->p_com = NULL;
			free(com->commande);
			com->commande = NULL;
			return (1);
		}
		com->p_com->str = new_str;
	}
	com->commande = malloc(sizeof(t_args));
	if (!com->commande)
	{
		free_chars(com->p_com);
		com->p_com = NULL;
		return (1);
	}
	com->commande->str = ft_split(arr, ' ');
	if (!com->commande->str)
	{
		free(com->commande);
		com->commande = NULL;
		free_chars(com->p_com);
		com->p_com = NULL;
		return (1);
	}
	com->commande->next = NULL;
	return (0);
}

/* splits the commands into multiple chunks using the pipes */
int	commandeparser(char *arr, t_comm *com, t_env *env)
{
	if (parse_init(arr, com, env) != 0)
		return (1);
	if (parse_wildcards_list(com) != 0)
		return (1);
	if (parse_expand_vars(com, env) != 0)
		return (1);
	return (parse_finalize(com, arr));
}
