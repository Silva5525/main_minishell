/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 16:58:43 by wdegraf           #+#    #+#             */
/*   Updated: 2024/10/23 17:21:40 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief the flexible segment which is newly created.
/// @param arr the main minishell struct.
/// @return the segment.
t_arr	*flexible_seg(t_arr *arr)
{
	t_arr	*seg;

	if (!arr)
		return (NULL);
	seg = (t_arr *)malloc(sizeof(t_arr));
	if (!seg)
		error_free_exit(arr, "Error, malloc1 in flexible_seg\n");
	seg->ken = malloc(sizeof(t_to) * arr->max_size);
	if (!seg->ken)
		error_free_exit(arr, "Error, malloc2 in flexible_seg\n");
	seg->size = 0;
	seg->max_size = arr->max_size;
	seg->direktory = arr->direktory;
	seg->envp = arr->envp;
	seg->hold = NULL;
	seg->in_fd = arr->in_fd;
	seg->out_fd = arr->out_fd;
	seg->stat = arr->stat;
	seg->first_time = false;
	seg->stdin = arr->stdin;
	seg->pid = arr->pid;
	seg->seg_count = arr->seg_count;
	seg->seg = arr->seg;
	seg->wait = false;
	seg->redir = arr->redir;
	seg->arr = arr;
	return (seg);
}

/// @brief adds a token to a segment.
/// @param seg the segment to which the token is added.
/// @param arr the main minishell struct.
/// @param index the index of the token to be added.
void	add_token_to_seg(t_arr *seg, t_arr *arr, size_t index)
{
	t_to	*ken;

	ken = malloc(sizeof(t_to));
	if (!ken)
		error_free_exit(arr, "Error, malloc in add_token_to_seg\n");
	ken->str = malloc(sizeof(char *) * 2);
	if (!ken->str)
	{
		free(ken);
		error_free_exit(arr, "Error, malloc for ken in add_token_to_seg\n");
	}
	ken->str[0] = ft_strdup(arr->ken[index]->str[0]);
	if (!ken->str[0])
	{
		free(ken->str);
		free(ken);
		error_free_exit(arr, "Error, ft_strdup in add_token_to_seg\n");
	}
	ken->str[1] = NULL;
	ken->typ = arr->ken[index]->typ;
	catch_token(seg, ken);
	seg->size--;
}

/// @brief Checks if the current state is outside of quotes.
/// @param state the current quote state
/// (0 = outside, 1 = single quotes, 2 = double quotes)
/// @return true if outside quotes, false otherwise
bool	is_outside_quotes(int state)
{
	return (state == 0);
}

/// @brief Updates the quote state and returns whether the character
/// is inside quotes.
/// @param c the current character
/// @param state an integer representing the current state:
/// 0 means outside of quotes, 1 means inside single quotes, 2 means 
/// inside double quotes.
/// @return the updated state of whether the character is within quotes or not.
int	update_quote_state(char c, int state)
{
	if (c == '\'' && state == 0)
		return (1);
	else if (c == '\'' && state == 1)
		return (0);
	else if (c == '\"' && state == 0)
		return (2);
	else if (c == '\"' && state == 2)
		return (0);
	return (state);
}

/// @brief poointer and redirection token counter.
/// @param str the string to be checked.
/// @return the number of pipes and redirection tokens in the string.
int	p_r(const char *str)
{
	if (ft_strncmp(str, ">>", 2) == 0 || ft_strncmp(str, "<<", 2) == 0)
		return (2);
	else if (*str == '>' || *str == '<' || *str == '|')
		return (1);
	else
		return (0);
}

/// @brief counts the | and redirection tokens in the ken array.
/// so we know how many segments we need to create.
/// @param ken the array of tokens.
/// @return the number of segments needed. at least 1.
int	count_seg_for_ken(t_to **ken)
{
	int		seg_count;
	int		pr;
	size_t	i;

	seg_count = 1;
	i = 0;
	while (ken[i] != NULL && ken[i]->str[0] != NULL)
	{
		pr = p_r(ken[i]->str[0]);
		if (pr > 0)
			seg_count++;
		i++;
	}
	return (seg_count);
}

/// @brief creats a new segment and stores it in the segments array.
/// @param seg the array of segments.
/// @param arr the main minishell struct.
/// @param seg_count the current segment count.
void	create_new_seg(t_arr **seg, t_arr *arr, size_t seg_count)
{
	seg[seg_count] = flexible_seg(arr);
	if (!seg[seg_count])
		error_free_exit(arr, "Error, flexible_seg in create_new_seg\n");
}

/// @brief a producer that creates new segments for the minishell.
/// @param arr it holds all the data of the minishell.
/// we store the new segments in the arr->seg array.
void	new_to_ken_producer(t_arr *arr)
{
	t_arr	**segments;
	size_t	seg_count;
	size_t	i;
	size_t	j;
	int		p_fd[2];

	seg_count = 0;
	i = 0;
	j = count_seg_for_ken(arr->ken);
	segments = (t_arr **)malloc(sizeof(t_arr *) * (j + 1));
	if (!segments)
		error_free_exit(arr, "Error, malloc in new_to_ken_producer\n");
	create_new_seg(segments, arr, seg_count);
	while (i < arr->size)
	{
		if ((arr->ken[i]->typ == '>'))
		{
			if (arr->ken[++i] == NULL || arr->ken[i]->str[0] == NULL)
				error_free_exit(arr,
					"Error: No delimiter specified after '<<'");
			else
			{
				redir_out(segments[seg_count], arr, arr->ken[i]->str[0]);
				arr->redir = true;
				i++;
			}
			continue ;
		}
		else if ((arr->ken[i]->typ == 'A'))
		{
			if (arr->ken[++i] == NULL || arr->ken[i]->str[0] == NULL)
				error_free_exit(arr,
					"Error: No delimiter specified after '<<'");
			else
			{
				redir_append(segments[seg_count], arr, arr->ken[i]->str[0]);
				arr->redir = true;
				i++;
			}
			continue ;
		}
		else if ((arr->ken[i]->typ == '<'))
		{
			if (arr->ken[++i] == NULL || arr->ken[i]->str[0] == NULL)
				error_free_exit(arr,
					"Error: No delimiter specified after '<<'");
			else
			{
				redir_in(segments[seg_count], arr, arr->ken[i]->str[0]);
				arr->redir = true;
				i++;
			}
			continue ;
		}
		else if ((arr->ken[i]->typ == 'H'))
		{
			if (arr->ken[++i] == NULL || arr->ken[i]->str[0] == NULL)
				error_free_exit(arr,
					"Error: No delimiter specified after '<<'");
			else
			{
				redir_heardoc(segments[seg_count], arr, arr->ken[i]->str[0]);
				arr->redir = true;
				i++;
			}
			continue ;
		}
		else if (arr->ken[i]->typ == '|')
		{
			if (pipe(p_fd) < 0)
				error_free_exit(arr, "Error: Pipe failed");
			if (segments[seg_count]->out_fd == STDOUT_FILENO)
				segments[seg_count]->out_fd = p_fd[1];
			else
				close(p_fd[1]);
			create_new_seg(segments, arr, ++seg_count);
			if (segments[seg_count]->in_fd == STDIN_FILENO)
				segments[seg_count]->in_fd = p_fd[0];
			else
				close(p_fd[0]);
			i++;
			arr->redir = false;
			continue ;
		}
		else
		{
			add_token_to_seg(segments[seg_count], arr, i);
			segments[seg_count]->size++;
			segments[seg_count]->ken[segments[seg_count]->size] = NULL;
			segments[seg_count]->seg_count = j;
			i++;
			continue ;
		}
	}
	segments[seg_count + 1] = NULL;
	arr->seg_count = seg_count;
	arr->seg = segments;
}
