/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_all.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amabbadi <amabbadi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 05:02:18 by azahid            #+#    #+#             */
/*   Updated: 2025/04/25 15:39:04 by amabbadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

void	exit_prog(t_comm *com)
{
	int		status;
	t_env	*env;
	int		exit_code;

	status = 0;
	env = com ? com->env : NULL;
	if (env)
		status = env->exit_status;
	if (com && com->p_com && com->p_com->next)
		validate_exit_args(com, com->p_com->next, &status);
	exit_code = 1;
	if (WIFEXITED(status))
		exit_code = WEXITSTATUS(status);
	else
		exit_code = status;
	if (env)
		env->exit_status = exit_code;
	exit(exit_code);
}


int	exec_builtin(t_comm *com)
{
	int	ret;

	if (!com || !com->p_com || !com->p_com->str || !com->env)
	{
		if (com && com->env)
			com->env->exit_status = 1;
		return (1);
	}
	if (!ft_strcmp(com->p_com->str, "pwd"))
		return (handle_pwd(com));
	else if (!ft_strcmp(com->p_com->str, "exit"))
	{
		exit_prog(com);
		return (0);
	}
	else if (!ft_strcmp(com->p_com->str, "unset"))
		return (handle_unset(com));
	else if (!ft_strcmp(com->p_com->str, "cd"))
		return (handle_cd(com));
	else if (!ft_strcmp(com->p_com->str, "echo"))
	{
		ret = echo(com);
		com->env->exit_status = ret;
		return (ret);
	}
	else if (!ft_strcmp(com->p_com->str, "env"))
	{
		env(com);
		com->env->exit_status = 0;
		return (0);
	}
	else if (!ft_strcmp(com->p_com->str, "export"))
		return (handle_export(com));
	com->env->exit_status = 1;
	return (1);
}

int	check_builtin(t_comm *com)
{
	if (!com || !com->p_com || !com->p_com->str)
		return (1);
	if (!ft_strcmp(com->p_com->str, "pwd"))
		return (0);
	else if (!ft_strcmp(com->p_com->str, "exit"))
		return (0);
	else if (!ft_strcmp(com->p_com->str, "unset"))
		return (0);
	else if (!ft_strcmp(com->p_com->str, "cd"))
		return (0);
	else if (!ft_strcmp(com->p_com->str, "echo"))
		return (0);
	else if (!ft_strcmp(com->p_com->str, "env"))
		return (0);
	else if (!ft_strcmp(com->p_com->str, "export"))
		return (0);
	return (1);
}

int	execute_all(t_comm *coms, char **envp, int size)
{
	int		ret;
	int		pipes[2 * (size - 1)];
	int		pids[size];
	int		status;
	int		printed_error;
	int		in;
	int		out;
	int		redir_status;
	int		sin;
	int		sout;
	char	*failed_file;

	if (!coms || size <= 0)
	{
		if (coms && coms[0].env)
			coms[0].env->exit_status = 1;
		free2d(envp);
		return (1);
	}
	if (size == 1 && coms[0].p_com && coms[0].p_com->str && !check_builtin(coms)
		&& !coms[0].flag)
	{
		in = 0;
		out = 0;
		sin = dup(0);
		sout = dup(1);
		failed_file = NULL;
		printed_error = 0;
		redir_status = handle_redirections(coms, 0, &in, &out, &failed_file,
				&printed_error);
		if (redir_status == -1)
		{
			if (!printed_error)
			{
				fprintf(stderr, "minishell: %s: No such file or directory\n",
					failed_file ? failed_file : "unknown");
			}
			if (coms[0].env)
				coms[0].env->exit_status = 1;
			free2d(envp);
			exit(1);
		}
		else if (redir_status != 0)
		{
			if (coms[0].env)
				coms[0].env->exit_status = 1;
			free2d(envp);
			return (1);
		}
		ret = exec_builtin(coms);
		close(0);
		free2d(envp);
		dup2(sin, 0);
		dup2(sout, 1);
		return (coms[0].env ? coms[0].env->exit_status : ret);
	}
	if (setup_pipes(pipes, size, coms, envp))
		return (1);
	for (int i = 0; i < size; i++)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		{
			perror("minishell:");
			for (int j = 0; j < 2 * (size - 1); j++)
				close(pipes[j]);
			if (coms && coms[0].env)
				coms[0].env->exit_status = 1;
			free2d(envp);
			return (1);
		}
		if (pids[i] == 0)
			handle_child_process(coms, i, size, pipes, envp);
	}
	for (int i = 0; i < 2 * (size - 1); i++)
		close(pipes[i]);
	status = wait_for_children(pids, size, coms);
	free2d(envp);
	return (coms && coms[0].env ? coms[0].env->exit_status : status);
}
