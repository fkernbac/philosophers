/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkernbac <fkernbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 14:43:01 by fkernbac          #+#    #+#             */
/*   Updated: 2022/11/05 21:52:54 by fkernbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//Reads the shutdown variable protected by a mutex.
int	check(t_table *table)
{
	int	termination;

	termination = 0;
	sem_wait(table->shutdown_mutex);
	termination = table->shutdown;
	sem_post(table->shutdown_mutex);
	return (termination);
}

//Adds meal and checks meal goal. Increments meal semaphore.
int	am_i_full(t_philo *philo)
{
	if (philo->table->mandatory_eat < 0)
		return (0);
	philo->meal_nr++;
	if (philo->meal_nr >= philo->table->mandatory_eat)
		return (sem_post(philo->table->full), 1);
	return (0);
}

int	set_death(t_philo *philo, t_table *table)
{
	int	i;
	int	time_of_death;

	i = 0;
	sem_wait(table->output_mutex);
	if (check(table) != 0)
		return (sem_post(table->output_mutex), 1);
	time_of_death = time_since(table->start_time);
	while (i++ < table->nr_philos)
	{
		sem_post(table->terminate);
		sem_post(table->full);
	}
	printf("%i %i died\n", time_of_death, philo->number);
	sem_wait(table->shutdown_mutex);
	table->shutdown = 1;
	sem_post(table->shutdown_mutex);
	millisleep(table->eat_time + table->sleep_time);
	sem_post(table->output_mutex);
	return (0);
}

//Checks death. Opens termination and meal semaphore for threads to end.
//Sets child process shutdown variable.
//Sets exit status to failure.
int	am_i_dead(t_philo *philo)
{
	t_table	*table;
	int		time;

	table = philo->table;
	sem_wait(table->shutdown_mutex);
	time = time_since(philo->meal_time);
	sem_post(table->shutdown_mutex);
	if (time >= table->die_time)
		return (set_death(philo, table), 1);
	return (0);
}
