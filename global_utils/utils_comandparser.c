#include "../minishell.h"

/* this pushes a string into a linked list , pretty basic hh*/
int	push_to_list(t_chars **head, char *str, int type)
{
	t_chars	*new_node;
	t_chars	*temp;

	new_node = malloc(sizeof(t_chars));
	if (!new_node)
		return (1);
	new_node->str = str;
	new_node->type = type;
	if (!new_node->str)
	{
		free(new_node);
		return (1);
	}
	new_node->next = NULL;
	if (*head == NULL)
		*head = new_node;
	else
	{
		temp = *head;
		while (temp->next)
			temp = temp->next;
		temp->next = new_node;
	}
	return (0);
}

/* gets the whole command and stores it inside t_comm struct ,
	its stored in the heap
the address is given to a variable inside the struct called "commande"*/
void	get_full_command(t_comm *com, char *prompt)
{
	t_args	*new;

	if (!prompt || !*prompt)
	{
		com->commande = NULL;
		return ;
	}
	new = malloc(sizeof(t_args));
	if (!new)
	{
		com->commande = NULL;
		return ;
	}
	new->str = ft_split(prompt, ' ');
	new->next = NULL;
	com->commande = new;
}

/* sets all values to zero for no problem in pushing elements */
void	setter(t_comm *com)
{
	com->commande = NULL;
	com->p_com = NULL;
	com->heardoc = NULL;
	com->redirections = NULL;
	com->env = NULL;
	com->infile = -1;
	com->outfile = -1;
	com->flag = 0;
}

int	parse_init(char *arr, t_comm *com, t_env *env)
{
	int	status;

	if (!arr || !com)
		return (-1);
	setter(com);
	com->p_com = p_com_split(arr, com);
	status = loader(arr, com, env);
	if (status == -1)
		return (1);
	com->env = env;
	if (!com->p_com || !com->p_com->str)
	{
		free_chars(com->p_com);
		com->p_com = NULL;
		return (1);
	}
	return (0);
}
