/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkernbac <fkernbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 21:02:55 by fkernbac          #+#    #+#             */
/*   Updated: 2022/11/03 18:03:00 by fkernbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//Checks if any philosopher died.
int	dead_already(t_table *t)
{
	int		i;
	int		time;
	t_philo	*p;

	i = -1;
	p = t->philosopher;
	while (++i < t->nr_philos)
	{
		pthread_mutex_lock(&p[i].meal_time_mutex);
		time = time_since(p[i].meal_time);
		pthread_mutex_unlock(&p[i].meal_time_mutex);
		if (time >= t->die_time)
		{
			time = time_since(t->start_time);
			if (set_death(t) == 0)
			{
				usleep(1000);
				pthread_mutex_lock(&t->output_mutex);
				printf("%i %i died\n", time, p[i].number);
				pthread_mutex_unlock(&t->output_mutex);
			}
			return (1);
		}
	}
	return (0);
}

//Checks if every philosopher has reached mandatory meals.
int	full_already(t_table *t)
{
	int		i;
	t_philo	*p;
	int		full;

	i = -1;
	p = t->philosopher;
	full = 0;
	while (++i < t->nr_philos)
	{
		pthread_mutex_lock(&p[i].meal_nr_mutex);
		if (p[i].meal_nr >= t->mandatory_eat)
			full++;
		pthread_mutex_unlock(&p[i].meal_nr_mutex);
	}
	if (full >= t->nr_philos)
	{
		pthread_mutex_lock(&t->term_mutex);
		t->terminate = 1;
		pthread_mutex_unlock(&t->term_mutex);
		return (1);
	}
	return (0);
}

//Monitors threads if mandatory eat is set.
void	monitor_meals(t_table *t)
{
	int		i;

	i = -1;
	while (full_already(t) == 0 && check_termination(t) == 0 \
		&& dead_already(t) == 0)
		usleep(5000);
	while (++i < t->nr_philos)
		pthread_join(t->philosopher[i].id, NULL);
}

//Monitors threads and wats for any philosopher to die.
void	monitor(t_table *t)
{
	int		i;

	i = -1;
	while (check_termination(t) == 0 && dead_already(t) == 0)
		usleep(5000);
	while (++i < t->nr_philos)
		pthread_join(t->philosopher[i].id, NULL);
}

int	main(int argc, char **argv)
{
	t_table	*t;

	if (argc < 5 || argc > 6 || (argc == 6 && atoi_check(argv[5]) < 1))
		return (error(1), EXIT_FAILURE);
	t = NULL;
	t = init_table(argc, argv);
	if (t == NULL || fill_table(t) < 0)
		return (EXIT_FAILURE);
	if (argc == 6)
		monitor_meals(t);
	else
		monitor(t);
	return (terminate(t), EXIT_SUCCESS);
}
