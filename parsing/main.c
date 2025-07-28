/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Amine0x0 <Amine0x0@student.1337.ma>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 20:43:43 by azahid            #+#    #+#             */
/*   Updated: 2025/07/28 03:56:05 by Amine0x0         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// ONLY ONE GLOBAL VARIABLE
volatile sig_atomic_t	g_signal = 0;


int	main(int ac, char **av, char *envp[])
{
	char	*input;
	t_env	*env;
	char	*prompt;
	int		status;

	(void)ac;
	(void)av;
	env = initialize_shell(envp);
	if (!env)
		return (1);
	setup_signals();
	while (1)
	{
		if (g_signal == SIGINT)
		{
			g_signal = 0;
			env->exit_status = 130;
		}
		prompt = get_prompt();
		if (!prompt)
			break ;
		input = read_shell_input(prompt);
		free(prompt);
		if (!input)
			break ;
		if (is_void(input))
		{
			free(input);
			continue ;
		}
		process_shell_input(input, envp, env);
		free(input);
	}
	status = env ? env->exit_status : 0;
	cleanup_shell(env);
	return (status);
}