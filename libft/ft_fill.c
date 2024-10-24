/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fill.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 19:41:01 by wdegraf           #+#    #+#             */
/*   Updated: 2024/10/23 16:23:13 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	flood_l(char **tab, int x, int y, char target)
{
	if (x < 0 || y < 0 || tab[y] == NULL || tab[y][x] == '\0')
		return ;
	if (tab[y][x] == 'F' || tab[y][x] != target)
		return ;
	tab[y][x] = 'F';
	flood_l(tab, x + 1, y, target);
	flood_l(tab, x - 1, y, target);
	flood_l(tab, x, y + 1, target);
	flood_l(tab, x, y - 1, target);
}

void	ft_fill(char **tab, t_xy size, t_xy begin)
{
	char	target;

	if (begin.x < 0 || begin.x >= size.x || begin.y < 0 || begin.y >= size.y)
		return ;
	target = tab[begin.y][begin.x];
	flood_l(tab, begin.x, begin.y, target);
}

/// TEST MAIN ///#######################################################
// #include <stdlib.h>
// #include <stdio.h>

// char **make_area(char **zone, t_xy size)
// {
// 	char	**new;
// 	int		i;
// 	int		j;
// 	new = malloc(sizeof(char*) * size.y);
// 	i = 0;
// 	while (i < size.y)
// 	{
// 		j = 0;
// 		new[i] = malloc(size.x + 1);
// 		while (j < size.x)
// 		{
// 			new[i][j] = zone[i][j];
// 			j++;
// 		}
// 		new[i][size.x] = '\0';
// 		i++;
// 	}
// 	return new;
// }

// int main(void)
// {
// 	int i;
// 	t_xy size = {8, 6};
// 	t_xy begin = {6, 1};
// 	char *zone[] = 
// 	{
// 		"11111111",
// 		"10001001",
// 		"10010001",
// 		"10110001",
// 		"11100001",
// 		"11111111"
// 	};
// 	char** area = make_area(zone, size);
// 	i = 0;
// 	while (i < size.y)
// 	{
// 		printf("%s\n", area[i]);
// 		i++;
// 	}
// 	printf("\n");
// 	flood_fill(area, size, begin);
// 	i = 0;
// 	while (i < size.y)
// 	{
// 		printf("%s\n", area[i]);
// 		i++;
// 	}
// 	return (EXIT_SUCCESS);
// }