/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkernbac <fkernbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/10 18:43:36 by fkernbac          #+#    #+#             */
/*   Updated: 2022/09/10 18:49:43 by fkernbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//Returns string as integer while checking for errors.
int	atoi_check(const char *s, t_table *table)
{
	int		i;
	long	n;
	int		sign;

	sign = 1;
	n = 0;
	i = 0;
	while (s[i] == ' ' || (s[i] >= 9 && s[i] <= 15))
		i++;
	if (s[i] == '-')
	{
		i++;
		sign = -1;
	}
	else if (s[i] == '+')
		i++;
	while (s[i] >= '0' && s[i] <= '9')
	{
		n = (n * 10) + s[i] - '0';
		i++;
	}
	n *= sign;
	if (n > INT_MAX || n < INT_MIN || s[i] != '\0')
		error(1, table);
	return ((int)n);
}
