/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_magnitude.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 12:42:33 by wdegraf           #+#    #+#             */
/*   Updated: 2024/11/19 13:18:06 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/// @brief used to calculate the magnitude of a complex number.
/// @param re real part of the complex number.
/// @param im imaginary part of the complex number.
/// @return the magnitude of the complex number.
/// @fn fabs() returns the absolute value of a floating point number.
/// @fn sqrt() returns the square root of a floating point number.
/// @attention hypot(re , im) would also do the trick but slower.. man hypot.
double	ft_magnitude(double re, double im)
{
	double	r;

	if (re == 0.0 && im == 0.0)
		return (0.0);
	if (re == 0.0 || im == 0.0)
		return (fabs(re) > fabs(im) ? fabs(re) : fabs(im));
	re = fabs(re);
	im = fabs(im);
	if (re > im)
	{
		r = im / re;
		return (re * sqrt(1.0 + r * r));
	}
	r = re / im;
	return (im * sqrt(1.0 + r * r));
}

// int	main(void)
// {
// 	printf("Magnitude of (3, 4): %f\n", ft_magnitude(3, 4));    // 5.000000
// 	printf("Magnitude of (0, 0): %f\n", ft_magnitude(0, 0));    // 0.000000
// 	printf("Magnitude of (5, 0): %f\n", ft_magnitude(5, 0));    // 5.000000
// 	printf("Magnitude of (0, -7): %f\n", ft_magnitude(0, -7));  // 7.000000
// 	return (0);
// }
