/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amabbadi <amabbadi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 00:51:00 by azahid            #+#    #+#             */
/*   Updated: 2025/04/25 15:22:47 by amabbadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <libc.h>
# include <ctype.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
#include <sys/stat.h>
# include <unistd.h>
#include <dirent.h>

# define DEFAULT_PATH "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"

/*Debug mode */
# ifdef DEBUG
#  define DEBUG_MODE 1
# else
#  define DEBUG_MODE 0
# endif

# ifdef DBG_ENV
#  define DBG_ENG 1
# else
#  define DBG_ENV 0
# endif

typedef struct s_ints
{
	int				i;
	struct s_ints	*next;
}					t_ints;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
	int				exit_status;
}					t_env;

typedef struct s_chars
{
	char			*str;
	struct s_chars	*next;
	int				type;
}					t_chars;

typedef struct s_args
{
	char			**str;
	struct s_args	*next;
}					t_args;

// struct holding every thing about a single commande after splted by pipe
typedef struct s_commande
{
	// parsed commande with the right attribute;
	t_chars			*p_com;
	// commande from the stdin
	t_args			*commande;
	t_chars			*redirections;
	// infile
	t_chars			*heardoc;
	// env
	t_env			*env;
	// last input
	char 			*raw_input;
	int				infile;
	// last output
	int				outfile;

  //flags when there is something other than red exists
  int flag;
}					t_comm;

int					parserlexer(char *input, char **envp, t_env *env);
t_comm				*arrayallocator(char **arr, t_env *env);
int					commandeparser(char *arr, t_comm *com, t_env *env);

/*			UTILITIES			*/

char				*ft_strnstr(const char *str, const char *t, size_t len);
char				*ft_strjoin(char const *s1, char const *s2);
char				**ft_split(char const *s, char c);
size_t				ft_strlen(const char *str);
char				*ft_strdup(const char *s1);
char				*ft_strchr(char *s, int c);
char				*ft_strndup(const char *s, size_t n);
void				*ft_memset(void *s, int c, size_t n);
int					ft_strcmp(char *s1, char *s2);
int					ft_strncmp(char *s1, char *s2, size_t n);
char				*ft_substr(char const *s, unsigned int start, size_t len);

/*		Parsing 		*/
void				get_full_command(t_comm *com, char *prompt);
void				fill_inputs(t_comm *com);
void				fill_outputs(t_comm *com);
int					ft_isspace(char c);
int					loader(char *str, t_comm *comm, t_env *env);
int					push_to_list(t_chars **head, char *str, int typ);
t_chars				*p_com_split(char *str, t_comm *com);
int					isquote(char c);
int					is_redirection(char c);
int					double_array_size(char **str);
char				*createargs(t_comm *com);
int					check_builtin(t_comm *com);
char				**pipe_split(char *str);
char				**parser(char *str, t_env *env, int flag, int type);
int					handle_redirections(t_comm *com, int i, int *redir_in, int *redir_out, char **failed_file, int *printed_error);
int 				is_syntax_error(char *input);
t_chars *parse_wildcards(char *wildin, t_comm *com);

/*			Execution			*/

int					execute(t_comm *com, char **envp);
int					cd(char *path, t_env *e);
char				*pwd(void);
int					unset(t_env **env, char *var);
int					echo(t_comm *com);
void				env(t_comm *cmd);
int					execute_all(t_comm *coms, char **envp, int size);
void				increment_shlvl(t_env *env);
void 				export(char *arg, t_env *env);

void exiter(char **exec);

/*		DEBUGGING		*/

void				print_t_comm(t_comm *cmd);
void				print_t_env(t_comm *cmd);

/*		FREE			*/
void				free1d(void *array);
void				free2d(char **array);
void				free_commande(t_comm *com);
void				free_all_commande(t_comm *comms, int size);
void				free_env(t_env *head);
void				free_args(t_args *args);
void				free_chars(t_chars *list);

/*	env   */
char				**parse_envp(t_env *e);
char				*joined(char *commande, char *sp);
char				*find_access(t_comm *com);

t_env				*push_env(t_env *head, char **new_env);
t_env				*env_list_from_array(char **env);
t_env				*create_env_node(char *env_str);
char				**envtodoublearr(t_env *e);

int					expand_variable(char *src, t_env *env, char *dest, int *si);


#endif
