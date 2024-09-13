/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_ex.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 20:01:12 by wdegraf           #+#    #+#             */
/*   Updated: 2024/09/13 16:18:56 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief executes the command and exits if it fails.
/// it uses access() from unistd.h to check F_OK ( = 0) if the file exists, and
/// X_OK( = 1) if the file is executable. Then uses execve to execute the 
/// command. execve replaces the current process with the new one and doesnt
/// return if it succeeds. otherwise it returns -1 and my write an error
/// message to stderr and exits the program.
/// @param order the command to execute. (order[0] is the command, the rest
/// are the arguments)
/// @param arr holds all the information about the minishell
void	do_execve(char **order, char ***order_exit, t_arr *arr)
{
	char	**dirs;
	char	*full_path;
	int		command_found;
	size_t	i;
	char	*hold;

	dirs = path_dir(arr);
	if (!dirs)
	{
		write(2, "Error, path_dir failed in do_execve\n", 36);
		mini_exit(order_exit, arr);
	}
	i = 0;
	command_found = 0;
	printf("Order[0]: %s\n", order[0]); /// debug
	while (dirs[i])
	{
		hold = ft_strjoin(dirs[i], "/");
		full_path = ft_strjoin(hold, order[0]);
		free(hold);
		if (access(full_path, F_OK) == 0 && access(full_path, X_OK) == 0)
		{
			command_found = 1;
			execve(full_path, order, arr->envp);
			write(2, "Error, execve in do_execve\n", 27);
			free(full_path);
			mini_exit(order_exit, arr);
		}
		free(full_path);
		i++;
		// full_path = ft_build_f_path(dirs[i], "/"); check this
	}
	if (!command_found)
		write(2, "Error, command not found\n", 25);
	mini_exit(order_exit, arr);
}

/// @brief ensures that the correct pipes are set up for input and output
/// between the commands. dup2(oldfd, newfd) makes sure that oldfd and newfd
/// point to the same underlying open file objekt even if they are different
/// file descriptors. 
/// @param pipefd pipe file descriptor [0]reading, [1]writing.
/// @param fd file descriptor for input redirection (stdin).
/// @param last_order Indicates if the current command is the last one in pipe.
void	set_pipe_and_fds(int pipefd[2], size_t fd, size_t last_order)
{
	if (!last_order)
		dup2(pipefd[1], STDOUT_FILENO);
	if (fd != 0)
		dup2(fd, STDIN_FILENO);
	close(pipefd[0]);
	close(pipefd[1]);
}

/// @brief handles the parent process after a fork. It waits for the child
/// process to finish with waitpid. If the child process exits normally
/// WIFEXITED returns true and WEXITSTATUS returns the exit status of the
/// child process. If the child process was terminated by a signal
/// WIFSIGNALED returns true and WTERMSIG returns the signal number and
/// adds 128 to it. Then it closes the writing end of the pipe.
/// @param pid The process id of the child process.
/// @param arr->stat Holds the status of the shell.
/// @param pipefd pipe file descriptor used in the current process.
void	do_parent(pid_t pid, t_arr *arr, int pipefd[2])
{
	if (waitpid(pid, &arr->stat, 0) == -1)
	{
		write(2, "Error, waitpid failed in do_parent\n", 35);
		mini_exit(NULL, arr);
	}
	else if (WIFEXITED(arr->stat))
		arr->stat = WEXITSTATUS(arr->stat);
	else if (WIFSIGNALED(arr->stat))
		arr->stat = 128 + WTERMSIG(arr->stat);
	close(pipefd[1]);
}

/// @brief Creates a pipe and a child process with fork. If pipe()
/// or fork() fails it writes an error message to stderr and exits the
/// program.
/// @param pipefd file descriptors for communication between the processes.
/// @param order here only used for freeing.
/// @param arr information about the minishell.
/// @return The process ID of the child process.
pid_t	do_child(int pipefd[2], char ***order, t_arr *arr)
{
	pid_t	pid;

	if (pipe(pipefd) < 0)
	{
		write(2, "Error, pipe failed in ex_pipe_order\n", 36);
		mini_exit(order, arr);
	}
	pid = fork();
	if (pid < 0)
	{
		write(2, "Error, fork failed in ex_pipe_order\n", 36);
		mini_exit(order, arr);
	}
	return (pid);
}

/// @brief Executes commands in a pipe. For each command in the pipe
/// it creates a child process with fork. The child process sets up the
/// the neccessary pipes and file descriptors and executes the command with
/// execve. The parent process waits for the child process to finish and
/// handles the next command in the pipe.
/// @param order list of commands to execute, each command is an array of
/// arguments.
/// @param arr all information about the minishell.
void	ex_pipe_order(char ***order, t_arr *arr)
{
	pid_t	pid;
	int		pipefd[2];
	size_t	fd;
	size_t	i;

	i = 0;
	fd = 0;
	while (order[i])
	{
		pid = do_child(pipefd, order, arr);
		if (pid == 0)
		{
			set_pipe_and_fds(pipefd, fd, i);
			do_execve(order[i], order, arr);
		}
		do_parent(pid, arr, pipefd);
		if (i != 0)
			close(fd);
		fd = pipefd[0];
		i++;
	}
}
