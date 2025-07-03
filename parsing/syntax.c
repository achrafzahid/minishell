#include "../minishell.h"

int	is_syntax_error(char *input)
{
	int in_single = 0;
	int in_double = 0;
	int opened_brace = 0;

	if (!input)
		return (0);
	while (*input == ' ')
		input++;
	if (*input == '|')
	{
		fprintf(stderr, "minishell: syntax error near unexpected token `|'\n");
		return (1);
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
				opened_brace = 1;
			if (*input == '}')
			{
				fprintf(stderr, "minishell: syntax error near unexpected token `}'\n");
				return (1);
			}
			if (*input == '|')
			{
				input++;
				while (*input == ' ')
					input++;
				if (*input == '|' || *input == '\0' || *input == '<')
				{
					fprintf(stderr, "minishell: syntax error near unexpected token `|'\n");
					return (1);
				}
				continue ;
			}
			if (*input == '>' || *input == '<')
			{
				char op = *input;
				char token[5] = {0};
				int count = 0;
				while (*input == op && count < 4)
					token[count++] = *input++;
				token[count] = 0;
				if (count > 2)
				{
					fprintf(stderr, "minishell: syntax error near unexpected token `%c'\n", op);
					return (1);
				}
				while (*input == ' ')
					input++;
				if (*input == '\0')
				{
					fprintf(stderr, "minishell: syntax error near unexpected token `newline'\n");
					return (1);
				}
				if (*input == '|' || *input == '>' || *input == '<')
				{
					if (*input == '|' && op == '<')
					{
						fprintf(stderr, "minishell: syntax error near unexpected token `|'\n");
						return (1);
					}
					if (*input == '>' || *input == '<')
					{
						char next_token[3] = {op, *input, 0};
						if (*input == op)
							fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n", next_token);
						else
							fprintf(stderr, "minishell: syntax error near unexpected token `%c'\n", *input);
						return (1);
					}
					fprintf(stderr, "minishell: syntax error near unexpected token `%c'\n", *input);
					return (1);
				}
				continue ;
			}
		}
		input++;
	}
	if (opened_brace)
	{
		fprintf(stderr, "minishell: syntax error: unexpected end of file\n");
		return (1);
	}
	if (in_single || in_double)
	{
		fprintf(stderr, "minishell: syntax error: unclosed quote\n");
		return (1);
	}
	return (0);
}