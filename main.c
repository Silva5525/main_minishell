/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:38:45 by wdegraf           #+#    #+#             */
/*   Updated: 2024/09/25 22:03:28 by wdegraf          ###   ########.fr       */
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
int	main_process(char *read, char **envp, bool first_time, t_arr *arr)
{
	char	*expand_r;

	if (!arr)
		return (EXIT_FAILURE);
	add_history(read);
	expand_r = expanding_env(read, envp, arr);
	if (!expand_r)
		return (free(read), write(
				2, "Error, expanding_env in main_process\n", 37), EXIT_FAILURE);
	to_ken_producer(expand_r, arr);
	free(expand_r);
	if (!arr)
		return (free(read), write(2,
				"Error, to_ken_producer in main_process\n", 39), EXIT_FAILURE);
	arr->first_time = first_time;
	alloc_envp(arr, envp);
	if (pipe_search(arr))
		do_pipe(arr);
	else
		redir(arr);
	free(read);
	return (EXIT_SUCCESS);
}

/// @brief helper function of the main_loop function.
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
	if (is_env_token(read))
		env_assign(read, &(arr->envp));
	else
		main_process(read, arr->envp, arr->first_time, arr);
	free_ken_str(arr, 0, 0);
	free_hold(arr, 0);
	arr->ken = malloc(sizeof(t_to *) * 16);
	if (!arr->ken)
		error_free_exit(arr, "Error, realloc in reset_arr\n");
	i = 0;
	while (i < 16)
		arr->ken[i++] = NULL;
	arr->size = 0;
	arr->max_size = 16;
}

int	main_loop(t_arr *arr, char *read, char *pwd)
{
	while (1)
	{
		pwd = direktory_minishell();
		if (!pwd)
			return (write(2, "Error, direktory_minishell in main\n", 35)
				, EXIT_FAILURE);
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

/// @brief 
/// @param argc Argument Count
/// @param argv Argument Vector
/// @param envp Environment Pointer (each string is a enviroment variable)
/// @return 
int	main(int argc, char **argv, char **envp)
{
	t_arr	*arr;

	(void)argc;
	(void)argv;
	signal(SIGINT, read_signal);
	signal(SIGQUIT, SIG_IGN);
	arr = flexible_arr();
	if (!arr)
		return (write(2, "ERROR, flexible_arr failed", 26), EXIT_FAILURE);
	alloc_envp(arr, envp);
	main_loop(arr, NULL, NULL);
	free_tokens(arr);
	return (write(1, "exit\n", 5), EXIT_SUCCESS);
}
