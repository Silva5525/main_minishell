/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 16:58:43 by wdegraf           #+#    #+#             */
/*   Updated: 2024/11/02 14:50:18 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief the flexible segment which is newly created.
/// @param arr the main minishell struct.
/// @return the segment.
t_arr	*flexible_seg(t_arr *arr)
{
	t_arr	*seg;

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
	seg->in_fd = STDIN_FILENO;
	seg->out_fd = STDOUT_FILENO;
	seg->stat = arr->stat;
	seg->first_time = false;
	seg->stdin = arr->stdin;
	seg->pid = arr->pid;
	seg->redir = false;
	seg->seg_count = arr->seg_count;
	seg->seg = arr->seg;
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
	if (*str == '|')
		return (1);
	else
		return (0);
}
