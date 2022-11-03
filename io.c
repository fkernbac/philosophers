/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkernbac <fkernbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 18:59:56 by fkernbac          #+#    #+#             */
/*   Updated: 2022/10/31 15:19:14 by fkernbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//Returns string as integer while checking for errors.
int	atoi_check(const char *s)
{
	int		i;
	long	n;

	n = 0;
	i = 0;
	while (s[i] == ' ' || (s[i] >= 9 && s[i] <= 15))
		i++;
	if (s[i] == '-')
		return (-1);
	while (s[i] >= '0' && s[i] <= '9')
	{
		n = (n * 10) + s[i] - '0';
		i++;
	}
	if (n > INT_MAX || n < 1 || s[i] != '\0')
		return (-1);
	return ((int)n);
}

//Prints the selected action while locking a mutex and checking for deaths.
int	print_action(t_philo *philo, char action)
{
	int	time;

	if (action != 'f' && action != 'e' && action != 's' && action != 't')
		return (0);
	pthread_mutex_lock(&philo->table->output_mutex);
	if (check_termination(philo->table) == 1)
		return (pthread_mutex_unlock(&philo->table->output_mutex), -1);
	time = time_since(philo->table->start_time);
	if (action == 'f')
		printf("%i %i has taken a fork\n", time, philo->number);
	else if (action == 'e')
		printf("%i %i has taken a fork\n%i %i is eating\n", \
			time, philo->number, time, philo->number);
	else if (action == 's')
		printf("%i %i is sleeping\n", time, philo->number);
	else if (action == 't')
		printf("%i %i is thinking\n", time, philo->number);
	pthread_mutex_unlock(&philo->table->output_mutex);
	return (0);
}
