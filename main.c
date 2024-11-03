/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:38:45 by wdegraf           #+#    #+#             */
/*   Updated: 2024/11/03 17:54:41 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief adds the input to the history. handles the environment variables
/// and the tokens. If the input is an environment variable it will be assigned
/// to the environment variables. After that it will produce tokens if needet
/// the segments and starts redirection, pipes the execution of the commands.
/// @param read the input from the readline.
/// @param envp the copied envirement variables.
/// @param first_time bool to check if the shell just startet.
/// @param arr the main minishell struct.
/// @return EXIT_SUCCESS if success, EXIT_FAILURE if failure.
int	main_process(char *read, char **envp, bool first_time, t_arr *arr)
{
	char	*expand_r;
	bool	is_envp;

	is_envp = false;
	if (!arr)
		return (EXIT_FAILURE);
	add_history(read);
	expand_r = expanding_env(read, envp, arr);
	if (!expand_r)
		return (free(read), write(
				2, "Error, expanding_env in main_process\n", 37), EXIT_FAILURE);
	is_envp = is_env_token(read);
	to_ken_producer(expand_r, arr);
	free(expand_r);
	if (!arr)
		return (write(2,
				"Error, to_ken_producer in main_process\n", 39), EXIT_FAILURE);
	if (is_envp)
		env_assign(read, &(arr->envp));
	free(read);
	new_to_ken_producer(arr);
	if (!is_envp)
		ex_redir(arr->seg, arr);
	arr->first_time = first_time;
	return (EXIT_SUCCESS);
}

/// @brief handles open quotes if needet starts the main_process, 
/// after it it refreshes the arr for the next input.
/// @param arr all information about the minishell.
/// @param read the input read from the readline.
void	reset_arr(t_arr *arr, char *read)
{
	int		i;
	char	*hold;

	if (has_open_quotes(read))
	{
		hold = unclosed_quotes(read);
		free(read);
		read = hold;
	}
	main_process(read, arr->envp, arr->first_time, arr);
	arr->ken = malloc(sizeof(t_to *) * 16);
	if (!arr->ken)
		error_free_exit(arr, "Error, realloc in reset_arr\n");
	i = 0;
	while (i < 16)
		arr->ken[i++] = NULL;
	arr->size = 0;
	arr->max_size = 16;
}

/// @brief the main loop of the minishell, it reads the input from the readline
/// and sets the prompt to the current working directory. If the input is NULL
/// it will exit the minishell. If the input is not NULL it will reset the
/// array and the tokens and then it will start the main process.
/// @param arr holds all data of the minishell.
/// @param read read from the readline.
/// @param pwd path to the current working directory.
/// @return the exit status of the minishell.
int	main_loop(t_arr *arr, char *read, char *pwd)
{
	while (1)
	{
		pwd = direktory_minishell();
		if (!pwd)
			return (write(2, "Error, direktory_minishell in main\n", 35),
				EXIT_FAILURE);
		read = readline(pwd);
		free(pwd);
		if (!read)
		{
			if (isatty(STDIN_FILENO))
				return (EXIT_SUCCESS);
			else
			{
				if (dup2(arr->stdin, STDIN_FILENO) == -1)
					return (write(2,
							"Error, dup2 failed in main\n", 28), EXIT_FAILURE);
				continue ;
			}
		}
		reset_arr(arr, read);
	}
	return (EXIT_SUCCESS);
}

/// @brief the main function of the minishell, it sets the signals for
/// SIGINT and SIGQUIT, then it creates a flexible array and allocates the
/// environment variables to the arr->envp. After that it starts the main loop
/// and at the end it frees the tokens.
/// @param argc Argument Count
/// @param argv Argument Vector
/// @param envp Environment Pointer (each string is a enviroment variable)
/// @return Exit status of the minishell. 0 if success, 1 if failure.
int	main(int argc, char **argv, char **envp)
{
	t_arr	*arr;

	(void)argc;
	(void)argv;
	signal(SIGINT, read_signal);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTERM, read_signal);
	arr = flexible_arr(NULL);
	if (!arr)
		return (write(2, "ERROR, flexible_arr failed", 26), EXIT_FAILURE);
	alloc_envp(arr, envp);
	main_loop(arr, NULL, NULL);
	error_free_exit(arr, NULL);
}
