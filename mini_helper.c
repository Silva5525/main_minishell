/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 17:17:58 by wdegraf           #+#    #+#             */
/*   Updated: 2024/11/04 13:19:28 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief When user press Ctrl+C the input will be canceled
/// and the shell will present a new prompt on a new line.
/// @param sig if sig number is SIGINT it means you pressed Ctrl+C
/// @rl_on_new_line() Prepare Readline to read the next input
/// @rl_replace_line("", 0) clears the current line
/// @rl_redisplay() redisplays the prompt on a cleared line
void	read_signal(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (sig == SIGTERM)
		exit(*g_sig_exit);
}

/// @brief copies the environment variables to the arr->envp
/// @param arr holds all data
/// @param arr->envp the environment variables
/// @param envp the environment variables
/// @param i index of the environment variables
void	envp_copy(t_arr *arr, char **envp, size_t i)
{
	size_t	j;
	size_t	k;

	j = 0;
	k = 0;
	while (j < i)
	{
		arr->envp[j] = ft_strdup(envp[j]);
		if (!arr->envp[j])
		{
			write(2, "Error, malloc failed in alloc_envp\n", 35);
			while (k < j)
			{
				free(arr->envp[k]);
				arr->envp[k] = NULL;
				k++;
			}
			free(arr->envp);
			free_tokens(arr);
			exit(EXIT_FAILURE);
		}
		j++;
	}
	arr->envp[i] = NULL;
}

/// @brief allocates the environment variables to the arr->envp
/// @param arr holds all data
/// @param envp holds the environment variables
void	alloc_envp(t_arr *arr, char **envp)
{
	size_t	i;

	if (arr->first_time)
	{
		i = 0;
		while (envp[i])
			i++;
		arr->envp = (char **)malloc(sizeof(char *) * (i + 1));
		if (!arr->envp)
			error_free_exit(arr, "Error, malloc failed in alloc_envp\n");
		envp_copy(arr, envp, i);
		arr->first_time = false;
	}
}

/// @brief checks if the command is an absolute or relative path.
/// If the command is an absolute or relative path, the function attempts to
/// execute the command at that path. If the command is not found in the
/// directories listed in the PATH, the function returns EXIT_FAILURE.
/// @param arr all information about the minishell.
/// @param order the command to execute.
/// @param args the arguments to the command.
void	absolute_relative(t_arr *arr, char *order, char **args)
{
	if (arr->ken[0] == NULL || arr->ken[0]->str[0] == NULL)
		command_not_found(arr);
	if (order[0] == '/' || order[0] == '.')
	{
		try_order(order, args, arr);
		return ;
	}
	return ;
}
