/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkernbac <fkernbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 14:38:59 by fkernbac          #+#    #+#             */
/*   Updated: 2022/10/31 18:30:01 by fkernbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//Checks for philosopher death and if it is the first death.
int	am_i_dead(t_philo *philo)
{
	int	time;

	pthread_mutex_lock(&philo->meal_time_mutex);
	time = time_since(philo->meal_time);
	pthread_mutex_unlock(&philo->meal_time_mutex);
	if (time > philo->table->die_time)
	{
		pthread_mutex_lock(&philo->table->output_mutex);
		time = time_since(philo->table->start_time);
		if (set_death(philo->table) == 0)
		{
			millisleep(philo->table->eat_time + philo->table->sleep_time);
			printf("%i %i died\n", time, philo->number);
		}
		pthread_mutex_unlock(&philo->table->output_mutex);
		return (-1);
	}
	return (0);
}

void	add_meal(t_philo *philo)
{
	if (philo->table->mandatory_eat <= 0)
		return ;
	pthread_mutex_lock(&philo->meal_nr_mutex);
	philo->meal_nr++;
	pthread_mutex_unlock(&philo->meal_nr_mutex);
}

//Locks right and left fork. Returns -1 for thread termination.
int	eat(t_table *table, int i)
{
	t_philo			*p;
	pthread_mutex_t	*right;
	pthread_mutex_t	*left;

	p = table->philosopher;
	right = &table->fork[i];
	left = &table->fork[(i + 1) % table->nr_philos];
	if (i == 0)
	{
		right = left;
		left = &table->fork[i];
	}
	pthread_mutex_lock(right);
	if (print_action(p + i, 'f') != 0 || table->nr_philos == 1)
		return (unlock(right, NULL));
	pthread_mutex_lock(left);
	if (am_i_dead(p + i) != 0 || print_action(p + i, 'e') != 0)
		return (unlock(right, left), -1);
	pthread_mutex_lock(&p[i].meal_time_mutex);
	gettimeofday(&p[i].meal_time, NULL);
	pthread_mutex_unlock(&p[i].meal_time_mutex);
	add_meal(&p[i]);
	millisleep(table->eat_time);
	unlock(right, left);
	return (0);
}

//Checks if thread has to pause to allow other threads to grab forks.
void	pass(t_philo *philo)
{
	int	stop;

	pthread_mutex_lock(&philo->table->pass_mutex);
	stop = philo->table->pass;
	pthread_mutex_unlock(&philo->table->pass_mutex);
	if (stop != philo->number)
		return ;
	millisleep(philo->table->nr_philos / 2);
	stop++;
	if (stop > philo->table->nr_philos)
		stop = 1;
	pthread_mutex_lock(&philo->table->pass_mutex);
	philo->table->pass = stop;
	pthread_mutex_unlock(&philo->table->pass_mutex);
}

//Thread routine. Returns if eat, sleep or think fails.
void	*philo_start(void *arg)
{
	t_philo	*philo;
	int		even;

	philo = (t_philo *)arg;
	even = philo->table->nr_philos % 2;
	if (even != 0 && philo->number == philo->table->nr_philos)
		millisleep(philo->table->eat_time * 1.5);
	while (1)
	{
		if (eat(philo->table, philo->index) != 0)
			break ;
		if (print_action(philo, 's') != 0)
			break ;
		millisleep(philo->table->sleep_time);
		if (print_action(philo, 't') != 0)
			break ;
		if (even != 0)
			pass(philo);
	}
	return (NULL);
}
