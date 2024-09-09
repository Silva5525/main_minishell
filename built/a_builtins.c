/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_builtins.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 14:26:13 by wdegraf           #+#    #+#             */
/*   Updated: 2024/09/08 18:29:06 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/// @brief executes the command
/// @param arr holds all data
void	ex_order(t_arr *arr)
{
	pid_t	pid;
	int		stat;

	pid = fork();
	if (pid < 0)
	{
		free_tokens(arr);
		write(2, "Error, fork failed in ex_order\n", 31);
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		if (execve(arr->ken[0]->str[0], arr->ken[0]->str, arr->envp) == -1)
		{
			free_tokens(arr);
			write(2, "Error, execve failed in ex_order\n", 33);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		waitpid(pid, &stat, 0);
		if (WIFEXITED(stat))
			arr->stat = WEXITSTATUS(stat);
	}
}

void	builtin(t_arr *arr)
{
	size_t	i;
	t_b		built[8];

	built[0] = (t_b){"echo", b_echo};
	built[1] = (t_b){"cd", b_cd};
	built[2] = (t_b){"pwd", b_pwd};
	built[3] = (t_b){"export", b_export};
	built[4] = (t_b){"unset", b_unset};
	built[5] = (t_b){"env", b_env};
	built[6] = (t_b){"exit", b_exit};
	built[7] = (t_b){NULL, NULL};

	i = 0;
	while (built[i].name)
	{
		if (ft_strcmp(arr->ken[0]->str[0], built[i].name) == 0)
		{
			built[i].fun(arr);
			return ;
		}
		i++;
	}
	ex_order(arr);
}
