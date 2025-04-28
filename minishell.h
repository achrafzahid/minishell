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

// # include <libc.h>
# include <ctype.h>
# include <dirent.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

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
	char			*raw_input;
	int				infile;
	// last output
	int				outfile;

	// flags when there is something other than red exists
	int				flag;
}					t_comm;

/*function prototypes*/
// arrayallocator.c
t_comm				*arrayallocator(char **arr, t_env *env);
// commandeparser.c
int					commandeparser(char *arr, t_comm *com, t_env *env);
// hollyparser.c
char				**parser(char *str, t_env *env, int flag, int type);
int					expand_variable(char *src, t_env *env, char *dest, int *si);
// loader.c
int					loader(char *str, t_comm *comm, t_env *env);
// main.c
int					main(int ac, char **av, char *envp[]);
// p_com_split.c
t_chars				*p_com_split(char *str, t_comm *com);
int					count_checker(char *s);
// parserlexer.c
int					parserlexer(char *input, char **envp, t_env *env);
// pipesplit.c
char				**pipe_split(char *str);
// syntax.c
int					is_syntax_error(char *input);
// wildcards.c
t_chars				*parse_wildcards(char *wildin, t_comm *com);

// signals.c
void				handle_sigint(int sig);
void				handle_sigquit(int sig);
void				setup_signals(void);

// libft
void				*ft_memset(void *s, int c, size_t n);
char				**ft_split(char const *s, char c);
char				*ft_strcat(char *dest, const char *src);
char				*ft_strchr(char *s, int c);
int					ft_strcmp(char *s1, char *s2);
void				ft_strcpy(char *s1, char *s2);
char				*ft_strdup(const char *s1);
char				*ft_strndup(const char *s, size_t n);
char				*ft_strjoin(char const *s1, char const *s2);
int					ft_strncmp(char *s1, char *s2, size_t n);
char				*ft_strnstr(const char *str, const char *tofind,
						size_t len);
char				*ft_substr(char const *s, unsigned int start, size_t len);
size_t				ft_strlen(const char *str);
char				*ft_itoa(int n);
// utils_general.c
int					double_array_size(char **str);
void				skip_spaces(int *i, char *str);
int					ft_isspace(char c);
int					is_void(char *s);

// utils_leak.c
void				free1d(void *array);
void				free2d(char **arr);
void				free_chars(t_chars *list);
void				free_ints_list(t_ints *head);
void				free_commande(t_comm *com);
void				free_all_commande(t_comm *comms, int size);
void				free_env(t_env *head);

// utils_commandparser.c
int					push_to_list(t_chars **head, char *str, int type);
void				get_full_command(t_comm *com, char *prompt);
void				setter(t_comm *com);
int					parse_init(char *arr, t_comm *com, t_env *env);

// utils_hollyparser_quotes.c
int					is_squote(char c);
int					is_dquote(char c);
int					has_unbalanced_quotes(char *wrd);
int					handle_squotes(const char *wrd, int *i);
int					handle_dquotes(const char *wrd, int *i, t_env *env);

// utils_hollyparser.c
char				*get_env_value(char *key, t_env *env);
int					get_var_name(const char *wrd, char *var, int max_len,
						int *var_len);
int					expander_count(char *wrd, t_env *env);
int					cw(char *wrd, t_env *env, int type);
char				*fill_word(char *dest, char *src, t_env *env, int type,
						int *flag);

// utils_main.c
char				*get_prompt(void);
t_env				*initialize_shell(char *envp[]);
char				*read_shell_input(char *prompt);
void				process_shell_input(char *input, char *envp[], t_env *env);
void				cleanup_shell(t_env *env);

// utils_p_com_split.c
int					is_redirection(char c);
int					isquote(char c);
char				*alloc_word(char *des, int len, char *src);
char				**allocate(char **arr, char *str, int cc);
int					ft_wdlen(char *str);

// utils_process.c
int					setup_pipes(int *pipes, int size, t_comm *coms,
						char **envp);
int					handle_child_process(t_comm *coms, int i, int size,
						int *pipes, char **envp);
int					wait_for_children(int *pids, int size, t_comm *coms);

// utils_execute_all.c
int					is_useless_variable(char *raw_input, t_env *env);
void				validate_exit_args(t_comm *com, t_chars *p, int *status);
char				*get_next_word(const char *str);
char				**envtodoublearr(t_env *e);
char				**list_to_array(t_chars *list);

// utils_env.c
t_env				*create_env_node(char *env_str);
t_env				*push_env(t_env *head, char **new_env);
t_env				*env_list_from_array(char **env);

// utils_builtin_handle.c
int					handle_pwd(t_comm *com);
int					handle_cd(t_comm *com);
int					handle_unset(t_comm *com);
int					handle_export(t_comm *com);

// execute_all.c
void				exit_prog(t_comm *com);
int					exec_builtin(t_comm *com);
int					check_builtin(t_comm *com);
int					execute_all(t_comm *coms, char **envp, int size);

// find_access.c
char				**parse_envp(t_env *e);
char				*joined(char *commande, char *sp);
char				*find_access(t_comm *com);
char				*createargs(t_comm *com);

// redirections.c
int					is_quoted(const char *str);
int					process_heredoc(t_comm *com, const char *delimiter);
int					handle_redirections(t_comm *com, int i, int *redir_in,
						int *redir_out, char **failed_file, int *printed_error);

// shlvl.c
void				increment_shlvl(t_env *env);

// debug.c
void				print_t_comm(t_comm *cmd);
void				print_t_env(t_comm *cmd);

// cd.c
int					cd(char *path, t_env *e);

// echo.c
int					echo(t_comm *com);

// env.c
void				env(t_comm *cmd);

// exiter.c
void				exiter(char **exec);

// export.c
void				export(char *arg, t_env *env);

// pwd.c
char				*pwd(void);

// unset.c
int					unset(t_env **env, char *var);

#endif
