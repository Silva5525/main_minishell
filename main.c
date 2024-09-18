/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:38:45 by wdegraf           #+#    #+#             */
/*   Updated: 2024/09/17 15:13:20 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief the main process of the minishell.
/// it startets with creating a flexible array, then it adds the
/// input to the history. After that it expands the environment variables
/// using expanding_env. Then it produces the tokens using to_ken_producer.
/// After that it allocates the environment variables to the arr->envp.
/// Redirects with redir and checks if there is a pipe in the tokens, if so
/// it creates a pipe with do_pipe, else it executes the builtin command.
/// At the end it frees the tokens and the input.
/// @param read the input readet from the readline.
/// @param envp the environment variables.
/// @param first_time bool if true envp will be set to the envp (leak handling)
void	main_process(char *read, char **envp, bool first_time)
{
	t_arr	*arr;
	char	*expand_r;

	arr = flexible_arr();
	if (!arr)
		return ;
	add_history(read);
	expand_r = expanding_env(read, envp, arr);
	to_ken_producer(expand_r, arr);
	free(expand_r);
	if (!arr)
	{
		free(read);
		write(2, "Error, to_ken_producer in main_process\n", 39);
		return ;
	}
	arr->first_time = first_time;
	alloc_envp(arr, envp);
	if (pipe_search(arr))
		do_pipe(arr);
	else
		redir(arr);
	free_tokens(arr);
	free(read);
}

/// @brief saves the current working directory to the out string
/// and adds " minishell$ " to the end of the string.
/// @param pwd the current working directory.
/// @param out becoms the prompt string for the minishell.
char	*direktory_minishell2(char *pwd, char *out)
{
	const char	*ms;
	int			i;
	int			j;

	i = 0;
	while (pwd[i] && i < PATH_MAX)
	{
		out[i] = pwd[i];
		i++;
	}
	free(pwd);
	ms = " minishell$ ";
	j = 0;
	while (ms[j] && i < PATH_MAX + 10)
	{
		out[i] = ms[j];
		i++;
		j++;
	}
	out[i] = '\0';
	return (out);
}

/// @brief direktory_minishell creates the prompt for the minishell
/// it gets the current working directory with getcwd and adds
/// " minishell$ " to the end of the string. so we allways know
/// where we are in the shell.
/// @return the prompt string for the minishell.
char	*direktory_minishell(void)
{
	char		*pwd;
	char		*out;

	out = (char *)malloc(sizeof(char) * (PATH_MAX + 11));
	if (!out)
		return (write(2, "Error, malloc failed in main\n", 30), NULL);
	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (write(2, "Error, getcwd failed in main\n", 30), NULL);
	return (direktory_minishell2(pwd, out));
}

int	main_loop(bool first_time, char **envp, char *read, char *pwd)
{
	char	*hold;

	while (1)
	{
		pwd = direktory_minishell();
		if (!pwd)
			return (write(2, "Error, direktory_minishell in main\n", 35)
				, EXIT_FAILURE);
		read = readline(pwd);
		free(pwd);
		if (!read)
			return (EXIT_SUCCESS);
		if (has_open_quotes(read))
		{
			hold = unclosed_quotes(read);
			free(read);
			read = hold;
		}
		if (is_env_token(read))
			env_assign(read, &envp);
		else
			main_process(read, envp, first_time);
		first_time = false;
	}
}

/// @brief 
/// @param argc Argument Count
/// @param argv Argument Vector
/// @param envp Environment Pointer (each string is a enviroment variable)
/// @return 
int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	signal(SIGINT, read_signal);
	signal(SIGQUIT, SIG_IGN);
	main_loop(true, envp, NULL, NULL);
	return (write(1, "exit\n", 5), EXIT_SUCCESS);
}
