#include "../minishell.h"

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
