#include "../minishell.h"

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
	int		in;
	int		out;
	char	**exec;
	int		ret;
	char	*failed_file;
	int		printed_error;

	in = 0;
	out = 0;
	failed_file = NULL;
	printed_error = 0;
	if (handle_redirections(coms, i, &in, &out, &failed_file,
			&printed_error) != 0)
	{
		if (!printed_error)
		{
			fprintf(stderr, "minishell: %s: No such file or directory\n",
				failed_file ? failed_file : "unknown");
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
			fprintf(stderr, "minishell: %s: No such file or directory\n",
				exec[0]);
		else
			fprintf(stderr, "minishell: %s: command not found\n", exec
				&& exec[0] ? exec[0] : "");
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

int	wait_for_children(int *pids, int size, t_comm *coms)
{
	int	status;
	int	broken_pipe;

	status = 0;
	broken_pipe = 0;
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
