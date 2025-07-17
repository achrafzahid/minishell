#include "../minishell.h"

void	increment_shlvl(t_env *env)
{
	int		shlvl;
	char	*tmp;

	while (env)
	{
		if (env->key && !strcmp(env->key, "SHLVL"))
		{
			if (!env->value)
				return ;
			shlvl = atoi(env->value);
			shlvl++;
			tmp = ft_itoa(shlvl);
			if (!tmp)
				return ;
			free(env->value);
			env->value = tmp;
			return ;
		}
		env = env->next;
	}
}