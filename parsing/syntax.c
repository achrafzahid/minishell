#include "../minishell.h"

int is_syntax_error(char *input)
{
	int in_single = 0;
	int in_double = 0;

	while (*input == ' ') input++;
	if (*input == '|')
	{
		fprintf(stderr, "minishell: syntax error near unexpected token `|'\n");
		return 1;
	}
	while (*input)
	{
		if (*input == '\'' && !in_double)
			in_single = !in_single;
		else if (*input == '\"' && !in_single)
			in_double = !in_double;

		if (!in_single && !in_double)
		{
            if (*input == '|')
            {
                input++;
                while (*input == ' ') input++;
                if (*input == '|' || *input == '\0' || *input == '<')
                {
                    fprintf(stderr, "minishell: syntax error near unexpected token `|'\n");
                    return 1;
                }
            }
			if (*input == '>' || *input == '<')
			{
				char op = *input;
				char token[5] = {0};
				int count = 0;

				while (*input == op && count < 4)
					token[count++] = *input++;

				token[count] = 0;
				while (*input == ' ') input++;

				if (*input == '\0' || *input == '|' || *input == '>' || *input == '<')
				{
					fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n", token);
					return 1;
				}
				continue;
			}
		}
		input++;
	}
	return 0;
}
