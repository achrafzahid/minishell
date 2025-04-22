
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
#include <stdio.h>

/* this checks for spaces in a character , pretty basic*/
int ft_isspace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
        || c == '\r');
}

/* this pushes a string into a linked list , pretty basic hh*/
int push_to_list(t_chars **head, char *str, int type)
{
    t_chars *new_node;
    t_chars *temp;

    new_node = malloc(sizeof(t_chars));
    if (!new_node)
        return (1);
    new_node->str = str;
    new_node->type = type;
    if (!new_node->str)
    {
        free(new_node);
        return (1);
    }
    new_node->next = NULL;
    if (*head == NULL)
        *head = new_node;
    else
    {
        temp = *head;
        while (temp->next)
            temp = temp->next;
        temp->next = new_node;
    }
    return (0);
}

/* gets the whole command and stores it inside t_comm struct ,
    its stored in the heap
the address is given to a variable inside the struct called "commande"*/
void get_full_command(t_comm *com, char *prompt)
{
    t_args *new;

    if (!prompt || !*prompt)
    {
        com->commande = NULL;
        return ;
    }
    new = malloc(sizeof(t_args));
    if (!new)
    {
        com->commande = NULL;
        return ;
    }
    new->str = ft_split(prompt, ' ');
    new->next = NULL;
    com->commande = new;
}

/* sets all values to zero for no problem in pushing elements */
void setter(t_comm *com)
{
    com->commande = NULL;
    com->p_com = NULL;
    com->heardoc = NULL;
    com->redirections = NULL;
    com->env = NULL;
    com->infile = -1;
    com->outfile = -1;
    com->flag = 0;
}

static int parse_init(char *arr, t_comm *com, t_env *env)
{
    int status;

    if (!arr || !com)
        return (-1);
    setter(com);
    com->p_com = p_com_split(arr,com);
    status = loader(arr, com, env);
    if (status == -1)
        return (1);
    com->env = env;
    if (!com->p_com || !com->p_com->str)
    {
        free_chars(com->p_com);
        com->p_com = NULL;
        return (1);
    }
    return (0);
}

static int parse_wildcards_list(t_comm *com)
{
    t_chars *curr = com->p_com;
    t_chars *prev = NULL;

    while (curr)
    {
        if (ft_strchr(curr->str, '*') || ft_strchr(curr->str, '?'))
        {
            t_chars *next = curr->next;
            t_chars *expanded = parse_wildcards(curr->str, com);
            if (!expanded)
            {
                free_chars(com->p_com);
                com->p_com = NULL;
                return (1);
            }
            t_chars *last = expanded;
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

static int parse_expand_vars(t_comm *com, t_env *env)
{
    t_chars *curr = com->p_com;
    t_chars *prev = NULL;

    while (curr)
    {
        char **tmp;
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
        t_chars *new_head = NULL;
        t_chars *new_tail = NULL;
        for (int j = 0; tmp[j]; j++)
        {
            t_chars *node = malloc(sizeof(t_chars));
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
        t_chars *nxt = curr->next;
        free(curr->str);
        free(curr);
        curr = nxt;
        prev = new_tail ? new_tail : prev;
        free2d(tmp);
    }
    return (0);
}


static int parse_finalize(t_comm *com, char *arr)
{
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
        char *new_str = createargs(com);
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
int commandeparser(char *arr, t_comm *com, t_env *env)
{
    if (parse_init(arr, com, env) != 0)
        return (1);
    if (parse_wildcards_list(com) != 0)
        return (1);
    if (parse_expand_vars(com, env) != 0)
        return (1);
    return parse_finalize(com, arr);
}
