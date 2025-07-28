#include "../minishell.h"

int	parserlexer(char *input, char **envp, t_env *env)
{
	int		size;
	char	**str;
	t_comm	*coms;
	int		syntax_error;

	size = 0;
	str = pipe_split(input);
	if (!str)
	{
		if (env)
			env->exit_status = 2;
		return (-1);
	}
	
	syntax_error = is_syntax_error(input);
	if (syntax_error)
	{
		free2d(str);
		if (env)
			env->exit_status = syntax_error; // Properly propagate syntax error code
		return (syntax_error);
	}
	
	coms = arrayallocator(str, env);
	if (!coms)
	{
		free2d(str);
		if (env)
			env->exit_status = 2;
		return (-1);
	}
	coms->raw_input = input;
	size = double_array_size(str);
	if (coms && coms->env)
		envp = envtodoublearr(coms->env);
	if (size > 0)
		execute_all(coms, envp, size);
	free2d(str);
	free_all_commande(coms, size);
	return (0);
}
