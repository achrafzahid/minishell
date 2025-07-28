#include "../minishell.h"

void	init_oldpwd(t_env **env)
{
	t_env	*oldpwd_node;
	t_env	*current;

	if (!env || !*env)
		return;
	
	current = *env;
	while (current)
	{
		if (current->key && ft_strcmp(current->key, "OLDPWD") == 0)
			return; 
		current = current->next;
	}
	
	// Create OLDPWD node
	oldpwd_node = malloc(sizeof(t_env));
	if (!oldpwd_node)
		return;
	oldpwd_node->key = ft_strdup("OLDPWD");
	oldpwd_node->value = NULL;
	oldpwd_node->next = NULL;
	oldpwd_node->exit_status = 0;
	
	// Add to end of env list
	current = *env;
	while (current->next)
		current = current->next;
	current->next = oldpwd_node;
}

t_env	*initialize_shell(char **envp)
{
	t_env	*env;

	env = env_list_from_array(envp);
	if (!env)
		return (NULL);
	increment_shlvl(env);
	init_oldpwd(&env);
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
	result = parserlexer(input, envp, env);
}

void	cleanup_shell(t_env *env)
{
	free_env(env);
	clear_history();
}