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

int	is_useless_variable(char *raw_input, t_env *env)
{
	char	*var_name;
	t_env	*tmp;

	if (!raw_input || raw_input[0] != '$')
		return (0);
	var_name = raw_input + 1;
	if (var_name[0] == '?' || var_name[0] == '\0')
		return (0);
	tmp = env;
	while (tmp)
	{
		if (tmp->key && ft_strcmp(tmp->key, var_name) == 0)
			return (0);
		tmp = tmp->next;
	}
	return (1);
}

void	validate_exit_args(t_comm *com, t_chars *p, int *status)
{
	char	*arg;
	int		i;

	arg = p->str;
	if (p->next)
	{
		fprintf(stderr, "minishell: exit: too many arguments\n");
		if (com->env)
			com->env->exit_status = 1;
		exit(com->env->exit_status);
	}
	i = 0;
	if (arg[0] == '+' || arg[0] == '-')
		i++;
	while (arg[i])
	{
		if (!isdigit(arg[i]))
		{
			fprintf(stderr, "minishell: exit: %s: numeric argument required\n",
				arg);
			if (com->env)
				com->env->exit_status = 2;
			exit(com->env->exit_status);
		}
		i++;
	}
	*status = atoi(arg);
}

void	exit_prog(t_comm *com)
{
	int		status;
	t_env	*env;

	status = 0;
	env = com ? com->env : NULL;
	if (env)
		status = env->exit_status;
	if (com && com->p_com && com->p_com->next)
		validate_exit_args(com, com->p_com->next, &status);
	if (env)
		env->exit_status = status;
	exit(status & 255);
}

char	*get_next_word(const char *str)
{
	int		i;
	int		start;
	char	*word;
	t_env	*env;

	i = 0;
	env = NULL;
	if (!str)
		return (NULL);
	while (str[i] && ft_isspace(str[i]))
		i++;
	if (!str[i])
		return (NULL);
	while (str[i] && !ft_isspace(str[i]))
		i++;
	while (str[i] && ft_isspace(str[i]))
		i++;
	start = i;
	while (str[i] && !ft_isspace(str[i]))
		i++;
	if (start == i)
		return (NULL);
	word = ft_strdup(str + start);
	if (!word)
	{
		printf("minishell: cannot allocate memory\n");
		if (env)
			env->exit_status = 1;
		return (NULL);
	}
	while (str[i] && ft_isspace(str[i]))
		i++;
	if (str[i] && !ft_isspace(str[i]))
	{
		free(word);
		printf("minishell: syntax error near unexpected token `%c'\n", str[i]);
		if (env)
			env->exit_status = 2;
		return (NULL);
	}
	return (word);
}

int	handle_pwd(t_comm *com)
{
	char	*path;

	path = pwd();
	if (!path)
	{
		printf("minishell: pwd: cannot get current directory\n");
		com->env->exit_status = 1;
		return (1);
	}
	printf("%s\n", path);
	free(path);
	com->env->exit_status = 0;
	return (0);
}

int	handle_cd(t_comm *com)
{
	char	*path;
	int		ret;

	path = com->p_com && com->p_com->next ? com->p_com->next->str : NULL;
	if (com->p_com->next && com->p_com->next->next)
	{
		fprintf(stderr, " too many arguments\n");
		com->env->exit_status = 1;
		return (1);
	}
	ret = cd(path, com->env);
	/*if (ret)
		fprintf(stderr, "minishell: cd: %s: No such file or directory\n",
			path ? path : "");*/
	com->env->exit_status = ret;
	return (ret);
}

int	handle_unset(t_comm *com)
{
	t_chars	*p;

	p = com->p_com->next;
	while (p && p->str)
	{
		unset(&com->env, p->str);
		p = p->next;
	}
	com->env->exit_status = 0;
	return (0);
}

int	handle_export(t_comm *com)
{
	if (com->p_com->next)
		export(com->p_com->next->str, com->env);
	else
		export(NULL, com->env);
	return (1);
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

char	**envtodoublearr(t_env *e)
{
	int		count;
	t_env	*tmp;
	char	**envp;
	int		i;
	char	*first_part;

	count = 0;
	tmp = e;
	while (tmp)
	{
		if (tmp->key && tmp->value)
			count++;
		tmp = tmp->next;
	}
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
	{
		printf("minishell: cannot allocate memory\n");
		if (e)
			e->exit_status = 1;
		return (NULL);
	}
	tmp = e;
	i = 0;
	while (tmp)
	{
		if (tmp->key && tmp->value)
		{
			first_part = ft_strjoin(tmp->key, "=");
			if (!first_part)
			{
				free2d(envp);
				printf("minishell: cannot allocate memory\n");
				tmp->exit_status = 1;
				return (NULL);
			}
			envp[i] = ft_strjoin(first_part, tmp->value);
			free(first_part);
			if (!envp[i])
			{
				free2d(envp);
				printf("minishell: cannot allocate memory\n");
				tmp->exit_status = 1;
				return (NULL);
			}
			i++;
		}
		tmp = tmp->next;
	}
	envp[i] = NULL;
	return (envp);
}

char	**list_to_array(t_chars *list)
{
	int		size;
	t_chars	*tmp;
	char	**array;
	int		i;
	t_chars	*tt;

	size = 0;
	tmp = list;
	while (tmp)
	{
		size++;
		tmp = tmp->next;
	}
	array = malloc(sizeof(char *) * (size + 1));
	if (!array)
		return (NULL);
	i = 0;
	tt = list;
	while (tt)
	{
		array[i] = ft_strdup(tt->str);
		i++;
		tt = tt->next;
	}
	array[i] = NULL;
	return (array);
}

int	setup_pipes(int *pipes, int size, t_comm *coms, char **envp)
{
	for (int i = 0; i < size - 1; i++)
	{
		if (pipe(pipes + i * 2) == -1)
		{
			printf("minishell: pipe: Resource temporarily unavailable\n");
			for (int j = 0; j < i * 2; j++)
				close(pipes[j]);
			if (coms && coms[0].env)
				coms[0].env->exit_status = 1;
			free2d(envp);
			return (1);
		}
	}
	return (0);
}

int	handle_child_process(t_comm *coms, int i, int size, int *pipes, char **envp)
{
	int		in = 0, out = 0;
	char	**exec;
	int		ret;
	char	*failed_file;
	int		printed_error;

	failed_file = NULL;
	printed_error = 0;
	if (handle_redirections(coms, i, &in, &out, &failed_file, &printed_error) != 0)
	{
		if (!printed_error)
		{
			fprintf(stderr, "minishell: %s: No such file or directory\n", failed_file ? failed_file : "unknown");
		}
		if (coms[i].env)
			coms[i].env->exit_status = 1;
		free2d(envp);
		exit(1);
	}
	if (i > 0 && !in)
		dup2(pipes[(i - 1) * 2], 0);
	if (i < size - 1 && !out)
		dup2(pipes[i * 2 + 1], 1);
	for (int j = 0; j < 2 * (size - 1); j++)
		close(pipes[j]);
	exec = list_to_array(coms[i].p_com);
	if ((!exec || !exec[0]) && !coms->flag)
	{
		if (is_useless_variable(coms->raw_input, coms[i].env))
		{
			free2d(envp);
			exit(0);
		}
		if (strchr(coms->raw_input, '/'))
			fprintf(stderr, "minishell: %s: No such file or directory\n", exec[0]);
		else
			fprintf(stderr, "minishell: %s: command not found\n", exec && exec[0] ? exec[0] : "");
		if (coms[i].env)
			coms[i].env->exit_status = 127;
		free2d(envp);
		exit(127);
	}
	else if (coms->flag && (in || out))
	{
		free2d(envp);
		exit(0);
	}
	if (!check_builtin(&coms[i]))
	{
		ret = exec_builtin(&coms[i]);
		free2d(envp);
		exit(coms[0].env ? coms[0].env->exit_status : ret);
	}
	execve(exec[0], exec, envp);
	exiter(exec);
	if (coms[i].env)
		coms[i].env->exit_status = 127;
	free2d(envp);
	exit(127);
}

int wait_for_children(int *pids, int size, t_comm *coms)
{
    int status = 0;
    int broken_pipe = 0;

    for (int i = 0; i < size; i++)
    {
        waitpid(pids[i], &status, 0);
        if (WIFEXITED(status))
        {
            status = WEXITSTATUS(status);
            if (status == 141 && size > 1 && !broken_pipe)
            {
                fprintf(stderr, "minishell: Broken pipe\n");
                broken_pipe = 1;
            }
        }
        else
        {
            status = 1;
        }
        if (coms && coms[0].env)
            coms[0].env->exit_status = status;
    }
    return (status);
}

int	execute_all(t_comm *coms, char **envp, int size)
{
	int	ret;
	int	pipes[2 * (size - 1)];
	int	pids[size];
	int	status;

	if (!coms || size <= 0)
	{
		if (coms && coms[0].env)
			coms[0].env->exit_status = 1;
		free2d(envp);
		return (1);
	}
	if (size == 1 && coms[0].p_com && coms[0].p_com->str
		&& !check_builtin(coms) && !coms[0].flag)
	{
  	char	*failed_file;
	int		printed_error;
	int		in = 0, out = 0;
    int sin,sout;
    sin = dup(0);
    sout = dup(1);
	failed_file = NULL;
	printed_error = 0;

	
	int redir_status = handle_redirections(coms, 0, &in, &out, &failed_file, &printed_error);
	if (redir_status == -1)  
	{
		if (!printed_error)
		{
			fprintf(stderr, "minishell: %s: No such file or directory\n", failed_file ? failed_file : "unknown");
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
		return 1;
	}
	ret = exec_builtin(coms);
      close(0);
		  free2d(envp);
      dup2(sin,0);
      dup2(sout,1);
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
