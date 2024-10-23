/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 15:20:37 by wdegraf           #+#    #+#             */
/*   Updated: 2024/10/23 16:04:52 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Retrieves the directories listed in Path environment variable.
/// Searches the environment variables in arr->envp for the PATH variable.
/// The directories are then split by ':' and returned as an array of strings.
/// @param arr all information about the minishell. (here the environment)
/// @return An array of strings containing the directories listed in the PATH 
/// or NULL if the PATH variable is not found.
char	**path_dir(t_arr *arr)
{
	char	*path;
	char	**dirs;
	size_t	i;

	i = 0;
	path = NULL;
	while (arr->envp[i])
	{
		if (ft_strncmp(arr->envp[i], "PATH=", 5) == 0)
		{
			path = arr->envp[i] + 5;
			break ;
		}
		i++;
	}
	if (!path || *path == '\0')
		return (write(2, "Error, PATH not found with path_dir\n", 36), NULL);
	dirs = ft_split(path, ':');
	if (!dirs)
		return (write(2, "Error, ft_split in path_dir\n", 28), NULL);
	return (dirs);
}

/// @brief Attempts to execute a command at the provided path.
/// If the command is executable, execve is called to execute the command.
/// If the command is not executable, the function returns EXIT_SUCCESS.
/// @param full_path path to the executable.
/// @param args Arguments to the command.
/// @param arr all information about the minishell. (here the environment)
/// @return nothing if success, EXIT_FAILURE if the command is not executable.
int	try_order(char *full_path, char **args, t_arr *arr)
{
	if (access(full_path, F_OK) != 0)
		return (EXIT_FAILURE);
	if (access(full_path, X_OK) == 0)
	{
		execve(full_path, args, arr->envp);
		write(2, "Error, execve in try_order\n", 27);
		return (EXIT_FAILURE);
	}
	write(2, "Error, permission denied in try_order\n", 38);
	return (EXIT_FAILURE);
}

/// @brief Tests if the command can be executed in the provided path.
/// If the command is executable, execve is called to execute the command.
/// If the command is not executable, the function returns 0.
/// @param full_path path to the executable.
/// @param args Arguments to the command.
/// @param arr all information about the minishell.
/// @return 0 for break, 1 for continue, 
static int	test_t_e_c(char *full_path, char **args, t_arr *arr)
{
	if (!full_path)
		return (EXIT_FAILURE);
	if (try_order(full_path, args, arr) == 0)
	{
		free(full_path);
		return (EXIT_SUCCESS);
	}
	free(full_path);
	return (EXIT_FAILURE);
}

/// @brief Trys to locate and execute the command in the directories listed in
/// the PATH.The function builds the full path for each directory in the PATH
/// tries to execute the command at that path. If the command is not found in
/// any of the directories, the function returns EXIT_FAILURE.
/// @param order Command to execute.
/// @param args Arguments to the command.
/// @param arr all information about the minishell.
/// @return EXIT_FAILURE if the command fails. nothing if success.
int	find_to_ex(char *order, char **args, t_arr *arr)
{
	char	**dirs;
	char	*full_path;
	size_t	i;

	absolute_relative(arr, order, args);
	dirs = path_dir(arr);
	if (!dirs)
		return (write(2, "Error, path_dir in find_to_ex.\n", 31), EXIT_FAILURE);
	i = 0;
	while (dirs && dirs[i])
	{
		full_path = ft_build_f_path(dirs[i], order);
		i++;
		if (test_t_e_c(full_path, args, arr) == 0)
			break ;
		else
			continue ;
	}
	if (!dirs[i])
		write(2, "Error, command not found in find_to_ex\n", 39);
	i = 0;
	while (dirs[i])
		free(dirs[i++]);
	free(dirs);
	return (EXIT_FAILURE);
}

/// @brief executes the command by forking a child process
/// then using execve to execute the command in the child process
/// and waits for the child process to finish
/// @param arr holds all data of the minishell
void	ex_order(t_arr *arr)
{
	pid_t	pid;
	int		stat;
	int		out_fd;
	int		in_fd;

	stat = 0;
	out_fd = dup(STDOUT_FILENO);
	in_fd = dup(STDIN_FILENO);
	if (out_fd < 0 || in_fd < 0)
		error_free_exit(arr, "Error, dup in ex_order\n");
	if (arr->in_fd != STDIN_FILENO)
	{
		if (dup2(arr->in_fd, STDIN_FILENO) < 0)
			error_free_exit(arr, "Error, 1dup2 in ex_order\n");
	}
	if (arr->out_fd != STDOUT_FILENO)
	{
		if (dup2(arr->out_fd, STDOUT_FILENO) < 0)
			error_free_exit(arr, "Error, 2dup2 in ex_order\n");
	}
	if (builtin(arr) == EXIT_SUCCESS)
	{
		if (dup2(in_fd, STDIN_FILENO) < 0)
			error_free_exit(arr, "Error, 3dup2 in ex_order\n");
		if (dup2(out_fd, STDOUT_FILENO) < 0)
			error_free_exit(arr, "Error, 4dup2 in ex_order\n");
		close(in_fd);
		close(out_fd);
		if (arr->in_fd != STDIN_FILENO)
			close(arr->in_fd);
		if (arr->out_fd != STDOUT_FILENO)
			close(arr->out_fd);
		return ;
	}
	pid = fork();
	if (pid < 0)
		error_free_exit(arr, "Error, fork in ex_order\n");
	else if (pid == 0)
	{
		if (arr->in_fd != STDIN_FILENO)
		{
			if (dup2(arr->in_fd, STDIN_FILENO) < 0)
				error_free_exit(arr, "Error, 5dup2 in ex_order\n");
			close(arr->in_fd);
		}
		if (arr->out_fd != STDOUT_FILENO)
		{
			if (dup2(arr->out_fd, STDOUT_FILENO) < 0)
				error_free_exit(arr, "Error, 6dup2 in ex_order\n");
			close(arr->out_fd);
		}
		if (find_to_ex(arr->ken[0]->str[0], arr->hold, arr) == -1)
			command_not_found(arr);
		exit(EXIT_FAILURE);
	}
	else
	{
		waitpid(pid, &stat, 0);
		if (WIFEXITED(stat))
			arr->stat = WEXITSTATUS(stat);
		else if (WIFSIGNALED(stat))
			arr->stat = 128 + WTERMSIG(stat);
		if (dup2(in_fd, STDIN_FILENO) < 0)
			error_free_exit(arr, "Error, 7dup2 in ex_order\n");
		if (dup2(out_fd, STDOUT_FILENO) < 0)
			error_free_exit(arr, "Error, 8dup2 in ex_order\n");
		close(in_fd);
		close(out_fd);
	}
}
