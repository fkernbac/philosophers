/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkernbac <fkernbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 21:02:55 by fkernbac          #+#    #+#             */
/*   Updated: 2022/09/13 20:17:06 by fkernbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//./philo 3 155 50 50

long	milliseconds(long seconds, long microseconds)
{
	long	milliseconds;

	milliseconds = seconds * 1000;
	milliseconds += microseconds / 1000;
	return (milliseconds);
}

long	get_time(struct timeval start)
{
	struct timeval	now;
	long			seconds;
	long			microseconds;

	gettimeofday(&now, NULL);
	seconds = now.tv_sec - start.tv_sec;
	microseconds = now.tv_usec - start.tv_usec;
	return (milliseconds(seconds, microseconds));
}

void	philo_think(t_table *table, int index)
{
	t_philo	philosopher;

	philosopher = table->philosopher[index];
	printf("%lu ms %i is thinking\n", get_time(table->start_time), philosopher.number);
	philo_eat(table, index);
}

void	philo_sleep(t_table *table, int index)
{
	t_philo	philosopher;

	philosopher = table->philosopher[index];
	printf("%lu ms %i is sleeping\n", get_time(table->start_time), philosopher.number);
	usleep(table->time_to_sleep * 1000);
	philo_think(table, index);
}

int	get_even(int i)
{
	if (i % 2 == 0)
		return (i);
	return (i + 1);
}

int	get_uneven(int i)
{
	if (i % 2 == 1)
		return (i);
	return (i + 1);
}

//version for usleep between philosophers
void	philo_eat(t_table *table, int index)
{
	t_philo			philosopher;

	philosopher = table->philosopher[index];
	if (pthread_mutex_lock(&table->fork[index]) == 0)
	{
		printf("%lu ms %i has taken a fork\n", get_time(table->start_time), philosopher.number);
		pthread_mutex_lock(&table->fork[(index + 1) % table->number_of_philosophers]);
		printf("%lu ms %i has taken a fork\n", get_time(table->start_time), philosopher.number);
	}
	printf("%lu ms %i is eating\n", get_time(table->start_time), philosopher.number);
	gettimeofday(&table->philosopher[index].last_meal, NULL);
	usleep(table->time_to_eat * 1000);
	pthread_mutex_unlock(&table->fork[index]);
	pthread_mutex_unlock(&table->fork[(index + 1) % table->number_of_philosophers]);
	table->philosopher[index].meals++;
	philo_sleep(table, index);
}

void	*philo_start(void *arg)
{
	t_philo	philosopher;

	philosopher = *(t_philo *)arg;
	philo_eat(philosopher.table, philosopher.index);
	return (NULL);
}

//Creating even philosopher indices from 0 to max.
void	create_even(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->number_of_philosophers)
	{
		// printf("Creating philosopher %i\n", i + 1);
		table->philosopher[i].alive = 1;
		table->philosopher[i].number = i + 1;
		table->philosopher[i].index = i;
		table->philosopher[i].meals = 0;
		table->philosopher[i].table = table;
		gettimeofday(&table->philosopher[i].last_meal, NULL);
		if (pthread_create(&table->philosopher[i].id, NULL, philo_start, &table->philosopher[i]) != 0)
			error(3, table);
		i += 2;
	}
}

//Creating uneven philosopher indices from uneven max to 1.
void	create_uneven(t_table *table)
{
	int	i;

	i = table->number_of_philosophers - 1;
	if (i % 2 == 0)
		i--;
	while (i > 0)
	{
		// printf("Creating philosopher %i\n", i + 1);
		table->philosopher[i].alive = 1;
		table->philosopher[i].number = i + 1;
		table->philosopher[i].index = i;
		table->philosopher[i].meals = 0;
		table->philosopher[i].table = table;
		gettimeofday(&table->philosopher[i].last_meal, NULL);
		if (pthread_create(&table->philosopher[i].id, NULL, philo_start, &table->philosopher[i]) != 0)
			error(3, table);
		i -= 2;
	}
}

void	fill_table(t_table *table)
{
	table->philosopher = malloc(sizeof(t_philo) * table->number_of_philosophers);
	if (table->philosopher == NULL)
		error(2, table);
	gettimeofday(&table->start_time, NULL);
	create_even(table);
	usleep(20);
	create_uneven(table);
}

t_table	*init_table(char *number, char *die, char *eat, char *sleep)
{
	t_table	*new;
	int		i;

	i = -1;
	new = malloc(sizeof(t_table));
	if (new == NULL)
		error(2, NULL);
	new->number_of_philosophers = atoi_check(number, new);
	new->time_to_die = atoi_check(die, new);
	new->time_to_eat = atoi_check(eat, new);
	new->time_to_sleep = atoi_check(sleep, new);
	new->fork = malloc(sizeof(pthread_mutex_t) * new->number_of_philosophers);
	if (new->fork == NULL)
		error(2, new);
	while (++i < new->number_of_philosophers)
	{
		if (pthread_mutex_init(&new->fork[i], NULL) != 0)
			error(2, new);
	}
	return (new);
}

int	dead_already(t_table *table)
{
	int				i;
	struct timeval	now;
	int				time_now;
	int				time_meal;

	i = -1;
	gettimeofday(&now, NULL);
	time_now = milliseconds(now.tv_sec, now.tv_usec);
	while (++i < table->number_of_philosophers)
	{
		time_meal = milliseconds(table->philosopher[i].last_meal.tv_sec, table->philosopher[i].last_meal.tv_usec);
		if (time_now - time_meal > table->time_to_die)
		{
			printf("%lu ms %i died\n", get_time(table->start_time), table->philosopher[i].number);
			terminate(table);
			return (1);
		}
	}
	return (0);
}

int	full_already(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->number_of_philosophers)
	{
		if (table->philosopher[i].meals < table->mandatory_eat)
			return (0);
		i++;
	}
	terminate(table);
	return (1);
}

//eat -> sleep -> think
int	main(int argc, char **argv)
{
	t_table	*table;
	int		i;

	i = -1;
	if (argc < 5 || argc > 6)
		error(1, NULL);
	table = init_table(argv[1], argv[2], argv[3], argv[4]);
	if (argc == 6)
		table->mandatory_eat = atoi_check(argv[5], table);
	else
		table->mandatory_eat = -1;
	fill_table(table);
	while (argc == 6 && dead_already(table) == 0 && full_already(table) == 0)
		usleep(5000);
	while (argc < 6 && dead_already(table) == 0)
		usleep(5000);
}

//check leaks for invalid arguments
