/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkernbac <fkernbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 14:36:48 by fkernbac          #+#    #+#             */
/*   Updated: 2022/11/03 16:05:13 by fkernbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* Converts seconds and microseconds to milliseconds. */
int	milliseconds(int seconds, int microseconds)
{
	int	milliseconds;

	milliseconds = seconds * 1000;
	milliseconds += microseconds / 1000;
	return (milliseconds);
}

/* Returns present time since start in milliseconds. */
int	time_since(struct timeval start)
{
	struct timeval	now;
	int				seconds;
	int				microseconds;

	gettimeofday(&now, NULL);
	seconds = now.tv_sec - start.tv_sec;
	microseconds = now.tv_usec - start.tv_usec;
	return (milliseconds(seconds, microseconds));
}

/* More accurate sleep function for milliseconds. */
void	millisleep(int milliseconds)
{
	struct timeval	start;
	int				time;

	gettimeofday(&start, NULL);
	usleep(milliseconds * 0.9);
	time = time_since(start);
	while (time < milliseconds)
	{
		usleep(1000);
		time = time_since(start);
	}
}
