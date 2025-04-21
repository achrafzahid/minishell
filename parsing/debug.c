/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azahid <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 23:54:21 by azahid            #+#    #+#             */
/*   Updated: 2025/04/16 18:38:24 by azahid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void print_t_comm(t_comm *cmd)
{
    t_chars *current_infile;
    t_args *current_args;
    int i;
    t_chars *s;

    if (cmd == NULL)
        return;
    printf("\033[1;34m===== Command Info =====\033[0m\n");
    printf("\033[1;32mExit Status: \033[1;33m%d\033[0m\n", cmd->env ? cmd->env->exit_status : -1);
    printf("\033[1;32mParsed Command (p_com):\033[0m ");
    s = cmd->p_com;
    if (cmd->p_com)
    {
        for (; s != NULL; s = s->next)
            printf("\033[1;33m[%s]%s\033[0m", s->str, s->next ? " → " : "");
    }
    else
        printf("\033[1;31m(empty)\033[0m");
    printf("\n");
    printf("\033[1;32mArguments (commande):\033[0m ");
    current_args = cmd->commande;
    if (!current_args)
        printf("\033[1;31m(empty)\033[0m");
    while (current_args)
    {
        i = 0;
        while (current_args->str && current_args->str[i])
        {
            printf("\033[1;33m[%s]\033[0m", current_args->str[i]);
            if (current_args->str[i + 1] || current_args->next)
                printf(" → ");
            i++;
        }
        current_args = current_args->next;
    }
    printf("\n");
    printf("\033[1;32mRedirections:\033[0m ");
    current_infile = cmd->redirections;
    if (!current_infile)
        printf("\033[1;31m(empty)\033[0m");
    while (current_infile)
    {
        if (current_infile->type == 0)
            printf("\033[1;36m[%s]\033[0m (\033[1;35minput (<)\033[0m)",
                current_infile->str);
        else if (current_infile->type == 1)
            printf("\033[1;36m[%s]\033[0m (\033[1;35moutput (>)\033[0m)",
                current_infile->str);
        else if (current_infile->type == 2)
            printf("\033[1;36m[%s]\033[0m (\033[1;35mheredoc (<<)\033[0m)",
                current_infile->str);
        else if (current_infile->type == 3)
            printf("\033[1;36m[%s]\033[0m (\033[1;35mappend (>>)\033[0m)",
                current_infile->str);
        if (current_infile->next)
            printf(" → ");
        current_infile = current_infile->next;
    }
    printf("\n");
}

void	print_t_env(t_comm *cmd)
{
	char	input;
	char	choice;
	t_env	*current_env;

	printf("\033[1;32mPrint environment variables? (y/n): \033[0m");
	scanf(" %c", &input);
	if (input == 'y' || input == 'Y')
	{
		printf("\033[1;32mSelect option:\033[0m\n");
		printf("\033[1;36m1. Keys only\n2. Values only\n3. Both keys and values\033[0m\n");
		scanf(" %c", &choice);
		printf("\033[1;34m===== Environment Variables =====\033[0m\n");
		current_env = cmd->env;
		if (!current_env)
			printf("\033[1;31m(empty)\033[0m\n");
		while (current_env)
		{
			if (choice == '1' && current_env->key)
				printf("\033[1;33m%s\033[0m", current_env->key);
			else if (choice == '2' && current_env->value)
				printf("\033[1;33m%s\033[0m", current_env->value);
			else if (choice == '3' && current_env->key && current_env->value)
				printf("\033[1;33m[%s=%s]\033[0m", current_env->key,
					current_env->value);
			else
				printf("\033[1;31m(invalid data)\033[0m");
			if (current_env->next)
				printf(" → ");
			current_env = current_env->next;
		}
		if (current_env == NULL && choice >= '1' && choice <= '3')
			printf("\n");
	}
}
