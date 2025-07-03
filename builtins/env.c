/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amabbadi <amabbadi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 02:06:07 by amabbadi          #+#    #+#             */
/*   Updated: 2025/04/25 15:21:10 by amabbadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	env(t_comm *cmd)
{
	t_env	*env;

	if (!cmd || !cmd->env)
		return ;
	if (cmd->p_com && cmd->p_com->next)
	{
		fprintf(stderr, "env: too many arguments\n");
		cmd->env->exit_status = 1;
		return ;
	}
	env = cmd->env;
	while (env)
	{
		if (env->key && env->value)
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
	cmd->env->exit_status = 0;
}
