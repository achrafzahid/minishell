/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvrk <dvrk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 20:43:43 by azahid            #+#    #+#             */
/*   Updated: 2025/04/19 08:40:25 by azahid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	main(int ac, char **av, char *envp[])
{
	char	*input;
	t_env	*env;
	char	*prompt;
	int		status;

	(void)ac;
	(void)av;
	// if (!isatty(0))
	// {
	// 	fprintf(stderr, "madirhach bia yakho hh\n");
	// 	return (1);
	// }
	env = initialize_shell(envp);
	while (42)
	{
		prompt = get_prompt();
		input = read_shell_input(prompt);
		free(prompt);
		if (!input)
			break ;
		if (is_void(input))
			continue ;
		process_shell_input(input, envp, env);
		free(input);
	}
	if (env)
		status = env->exit_status;
	else
		status = 0;
	cleanup_shell(env);
	return (status);
}
