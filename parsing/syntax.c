#include "../minishell.h"

int	is_syntax_error(char *input)
{
	int in_single = 0;
	int in_double = 0;

	if (!input)
		return (0);
	while (*input == ' ')
		input++;
	if (*input == '|')
	{
		fprintf(stderr, "minishell: syntax error near unexpected token `|'\n");
		return (2); 
	}
	if (*input == '>' || *input == '<')
	{
		fprintf(stderr, "minishell: syntax error near unexpected token `newline'\n");
		return (2);
	}
	while (*input)
	{
		if (*input == '\'' && !in_double)
			in_single = !in_single;
		else if (*input == '"' && !in_single)
			in_double = !in_double;
		if (!in_single && !in_double)
		{
			if (*input == '{')
			{
				fprintf(stderr, "minishell: syntax error near unexpected token `{'\n");
				return (2);
			}
			if (*input == '}')
			{
				fprintf(stderr, "minishell: syntax error near unexpected token `}'\n");
				return (2);
			}
			if (*input == '|')
			{
				input++;
				while (*input == ' ')
					input++;
				if (!*input)
				{
					fprintf(stderr, "minishell: syntax error near unexpected token `newline'\n");
					return (2);
				}
				if (*input == '|')
				{
					fprintf(stderr, "minishell: syntax error near unexpected token `|'\n");
					return (2);
				}
				if (*input == '>' || *input == '<')
				{
					fprintf(stderr, "minishell: syntax error near unexpected token `%c'\n", *input);
					return (2);
				}
				continue;
			}
			if (*input == '>' || *input == '<')
			{
				char op = *input;
				int count = 0;
				while (*input == op && count < 4)
				{
					count++;
					input++;
				}
				if (count > 2)
				{
					fprintf(stderr, "minishell: syntax error near unexpected token `%c'\n", op);
					return (2);
				}
				while (*input == ' ')
					input++;
				if (*input == '\0')
				{
					fprintf(stderr, "minishell: syntax error near unexpected token `newline'\n");
					return (2);
				}
				if (*input == '|')
				{
					fprintf(stderr, "minishell: syntax error near unexpected token `|'\n");
					return (2);
				}
				if (*input == '>' || *input == '<')
				{
					fprintf(stderr, "minishell: syntax error near unexpected token `%c'\n", *input);
					return (2);
				}
				continue;
			}
		}
		input++;
	}
	if (in_single || in_double)
	{
		fprintf(stderr, "minishell: unexpected end of file\n");
		return (2);
	}
	return (0);
}
