#include "../minishell.h"

int	handle_pwd(t_comm *com)
{
	char	*path;

	// pwd in bash ignores extra arguments and still prints PWD
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
		fprintf(stderr, "minishell: cd: too many arguments\n");
		// bash actually returns 1 for cd with too many arguments, not 0
		com->env->exit_status = 1;
		return (1);
	}
	ret = cd(path, com->env);
	com->env->exit_status = ret;
	return (ret);
}

int	handle_unset(t_comm *com)
{
	t_chars	*p;
	int		ret;
	int		any_error;

	any_error = 0;
	p = com->p_com->next;
	
	while (p && p->str)
	{
		ret = unset(&com->env, p->str);
		if (ret != 0)
			any_error = 1;
		p = p->next;
	}
	
	if (com && com->env)
		com->env->exit_status = any_error;
	return (any_error);
}

int	handle_export(t_comm *com)
{
	t_chars	*args;
	int		original_exit_status;

	args = com->p_com->next;
	original_exit_status = 0;
	
	if (!args)
	{
		export(NULL, com->env);
	}
	else
	{
		while (args)
		{
			export(args->str, com->env);
			if (com->env->exit_status == 1)
				original_exit_status = 1;
			args = args->next;
		}
		com->env->exit_status = original_exit_status;
	}
	return (com->env->exit_status);
}