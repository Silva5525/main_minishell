/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_helper2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 20:44:12 by wdegraf           #+#    #+#             */
/*   Updated: 2024/09/30 15:46:29 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

/// @brief This function prepares the command for execution by removing
/// redirection tokens (`>`, `>>`, `<` '<<') and file names from the token list.
/// @param arr Contains all information about the minishell.
/// @param i The current index of the redirection token.
void	remove_redir_token(t_arr *arr, size_t i)
{
	size_t	j;

	j = 0;
	if (arr->ken[i]->typ == '>' || arr->ken[i]->typ == 'A'
		|| arr->ken[i]->typ == '<' || arr->ken[i]->typ == 'H')
	{
		while (arr->ken[i]->str[j])
		{
			free(arr->ken[i]->str[j]);
			j++;
		}
		free(arr->ken[i]->str);
		free(arr->ken[i]);
		arr->ken[i] = NULL;
		free(arr->ken[i + 1]->str[0]);
		free(arr->ken[i + 1]->str);
		free(arr->ken[i + 1]);
		arr->ken[i + 1] = NULL;
	}
	j = i + 2;
	while (arr->size > i)
		arr->ken[i++] = arr->ken[j++];
	arr->size -= 2;
}

/// @brief counts the tokens without redirection tokens.
/// @param arr 
/// @param i 
/// @return 
size_t	count_without_redir(t_arr *arr, size_t i)
{
	while (arr->ken[i])
	{
		if (arr->ken[i]->typ == '>' || arr->ken[i]->typ == 'A'
			|| arr->ken[i]->typ == '<' || arr->ken[i]->typ == 'H')
		{
			remove_redir_token(arr, i);
			continue ;
		}
		i++;
	}
	return (i);
}

/// @brief  concates the command and the arguments to a new double pointer.
/// witout the redirection tokens.
/// @param arr the struct that holds all data.
/// @return the new double pointer with the command and the arguments.
char	**order_concate(t_arr *arr)
{
	size_t	i;
	size_t	j;
	char	**str;

	i = 0;
	j = 0;
	i = count_without_redir(arr, 0);
	str = malloc(sizeof(char *) * (i + 1));
	if (!str)
		return (write(2, "Error, malloc in order_concate\n", 31), NULL);
	while (j < i)
	{
		str[j] = ft_strdup(arr->ken[j]->str[0]);
		if (!str[j])
		{
			while (j > 0)
				free(str[j--]);
			free(str);
			return (write(2, "Error, ft_strdup in order_concate\n", 34), NULL);
		}
		j++;
	}
	str[j] = NULL;
	return (str);
}
