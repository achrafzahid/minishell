#include "../minishell.h"

t_env	*initialize_shell(char **envp)
{
	t_env	*env;

	env = env_list_from_array(envp);
	if (!env)
		return (NULL);
	increment_shlvl(env);
	return (env);
}

char	*get_prompt(void)
{
	return (ft_strdup("minishell$ "));
}

char	*read_shell_input(char *prompt)
{
	char	*input;

	input = readline(prompt);
	if (!input)
		return (NULL);
	if (*input)
		add_history(input);
	return (input);
}

void	process_shell_input(char *input, char **envp, t_env *env)
{
	int result;
	
	if (!input)
		return;
	
	// Don't pre-check syntax here, let parserlexer handle it properly
	result = parserlexer(input, envp, env);
	
	// The exit status should already be set by parserlexer
	// Don't override it here
}

void	cleanup_shell(t_env *env)
{
	free_env(env);
	clear_history();
}