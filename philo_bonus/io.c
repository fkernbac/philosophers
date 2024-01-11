/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkernbac <fkernbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 18:59:56 by fkernbac          #+#    #+#             */
/*   Updated: 2022/11/03 15:55:31 by fkernbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//Returns string as integer while checking for errors.
int	atoi_check(const char *s, t_table *table)
{
	int		i;
	long	n;

	n = 0;
	i = 0;
	while (s[i] == ' ' || (s[i] >= 9 && s[i] <= 15))
		i++;
	if (s[i] == '-')
		error(1, table);
	while (s[i] >= '0' && s[i] <= '9')
	{
		n = (n * 10) + s[i] - '0';
		i++;
	}
	if (n > INT_MAX || n < 1 || s[i] != '\0')
		error(1, table);
	return ((int)n);
}

/* Prints console output protected by the print semaphore.*/
int	print_action(t_philo *philo, char action)
{
	int	time;

	if (action != 'f' && action != 's' && action != 't')
		return (0);
	sem_wait(philo->table->output_mutex);
	if (check(philo->table) == 1)
		return (sem_post(philo->table->output_mutex), -1);
	time = time_since(philo->table->start_time);
	if (action == 'f')
		printf("%i %i has taken a fork\n", time, philo->number);
	else if (action == 's')
		printf("%i %i is sleeping\n", time, philo->number);
	else if (action == 't')
		printf("%i %i is thinking\n", time, philo->number);
	sem_post(philo->table->output_mutex);
	return (0);
}

/* Checks if the philosopher has died before printing the eat message.*/
int	print_action_eat(t_philo *philo, char action, int time)
{
	sem_wait(philo->table->output_mutex);
	if (check(philo->table) != 0)
		return (sem_post(philo->table->output_mutex), -1);
	if (action == 'e')
		printf("%i %i has taken a fork\n%i %i is eating\n", \
			time, philo->number, time, philo->number);
	sem_post(philo->table->output_mutex);
	return (0);
}
