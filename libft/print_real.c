/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_real_num.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 12:51:24 by wdegraf           #+#    #+#             */
/*   Updated: 2024/11/19 12:57:12 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <ieee754.h>
#include <float.h>

#define EPSILON DBL_EPSILON
#define LOG2_10 3.321928095

#define flt_zero(x) (fabs(x) < EPSILON)

int max_digs_rt = 3;  /* maximum # of 0's right of decimal before using
						scientific notation */
int max_digs_lf = 5;  /* max # of digits left of decimal */

/// @brief Prints the number specified by `r`. The formatting and precision 
/// are controlled by `width` and `dec`. 
/// If `width` is 0, the number is printed in the default format.
/// If `width` is not 0, the number is printed in a format with the 
/// specified width and decimal places.

/// @param r The number to be printed.
/// @param width The minimum field width for the output. If 0, no specific width is used.
/// @param dec The number of decimal places in the output.
void print_real(double r, int width, int dec)
{
	int mag;
	double fpart, temp;
	char format[8];
	char num_format[3] = {'l',0,0};
	union ieee754_double *dl;

	dl = (union ieee754_double*)&r;
	mag = (dl->ieee.exponent - IEEE754_DOUBLE_BIAS) / LOG2_10;
	if (r == 0)
		mag = 0;
	if ((mag > max_digs_lf-1) || (mag < -max_digs_rt)) {
		num_format[1] = 'e';
		temp = r/pow(10, mag);
		fpart = temp - floor(temp);
	}
	else {
		num_format[1] = 'f';
		fpart = r - floor(r);
	}
	if (flt_zero(fpart))
		dec = 0;
	if (width == 0) {
		snprintf(format, 8, "%%.%d%s", dec, num_format);
	}
	else {
		snprintf(format, 8, "%%%d.%d%s", width, dec, num_format);
	}
	printf(format, r);
}
