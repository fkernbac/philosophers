/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkernbac <fkernbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 14:38:59 by fkernbac          #+#    #+#             */
/*   Updated: 2022/11/03 16:04:31 by fkernbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//Takes two forks. Special case for 1 philosopher.
void	philo_eat(t_philo *philo, t_table *table)
{
	int	time;

	sem_wait(table->forks);
	if (print_action(philo, 'f'))
		terminate_child(table, 1);
	if (table->nr_philos == 1)
	{
		millisleep(table->die_time);
		sem_post(table->full);
		sem_post(table->terminate);
		terminate_child(table, 1);
	}
	sem_wait(table->forks);
	if (am_i_dead(philo) == 1)
		terminate_child(table, 2);
	sem_wait(table->shutdown_mutex);
	gettimeofday(&philo->meal_time, NULL);
	time = time_since(table->start_time) - time_since(philo->meal_time);
	sem_post(table->shutdown_mutex);
	if (print_action_timed(philo, 'e', time) != 0)
		terminate_child(table, 2);
	millisleep(table->eat_time);
	sem_post(table->forks);
	sem_post(table->forks);
}

//Waits for the termination semaphore to open. Sets shutdown variable.
void	*check_others(void *arg)
{
	t_table	*table;

	table = (t_table *)arg;
	sem_wait(table->terminate);
	sem_wait(table->shutdown_mutex);
	table->shutdown = 1;
	sem_post(table->shutdown_mutex);
	return (NULL);
}

//Checks if thread should die and sets shutdown variable.
void	*check_me(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (check(philo->table) == 0 && am_i_dead(philo) == 0)
		millisleep(5);
	sem_wait(philo->table->shutdown_mutex);
	philo->table->shutdown = 1;
	sem_post(philo->table->shutdown_mutex);
	return (NULL);
}

//Thread routine.
void	philo_start(void *arg)
{
	t_philo	*philo;
	t_table	*table;

	philo = (t_philo *)arg;
	table = philo->table;
	if (pthread_create(&table->term_check, NULL, check_others, table) != 0 \
		|| pthread_create(&table->starve_check, NULL, check_me, philo) != 0)
		error(5, table);
	while (1)
	{
		philo_eat(philo, table);
		am_i_full(philo);
		if (print_action(philo, 's') != 0)
			break ;
		millisleep(table->sleep_time);
		if (print_action(philo, 't') != 0)
			break ;
	}
	terminate_child(table, 0);
}
