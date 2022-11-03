/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkernbac <fkernbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 14:40:44 by fkernbac          #+#    #+#             */
/*   Updated: 2022/10/31 15:15:59 by fkernbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//Starts every second philosopher thread beginning at index start.
int	start_half(t_table *table, int start)
{
	t_philo	*p;

	p = table->philosopher;
	while (start < table->nr_philos)
	{
		if (pthread_create(&p[start].id, NULL, philo_start, &p[start]) != 0)
			return (error(3));
		start += 2;
	}
	return (0);
}

//Creates philosopher structs.
int	create_philosophers(t_table *table)
{
	int		i;
	t_philo	*p;

	i = 0;
	p = table->philosopher;
	while (i < table->nr_philos)
	{
		p[i].number = i + 1;
		p[i].index = i;
		p[i].table = table;
		p[i].meal_nr = 0;
		p[i].meal_time = table->start_time;
		if (pthread_mutex_init(&p[i].meal_time_mutex, NULL) != 0 \
			|| pthread_mutex_init(&p[i].meal_nr_mutex, NULL) != 0 \
			|| pthread_mutex_init(&table->fork[i], NULL) != 0)
			return (error(3));
		i++;
	}
	return (0);
}

//Creates philosophers and starts threads.
int	fill_table(t_table *table)
{
	table->philosopher = \
		malloc(sizeof(t_philo) * table->nr_philos);
	if (table->philosopher == NULL)
		return (error(2));
	gettimeofday(&table->start_time, NULL);
	if (create_philosophers(table) != 0 || start_half(table, 0) != 0)
		return (EXIT_FAILURE);
	millisleep(table->eat_time / 2);
	if (start_half(table, 1) != 0)
		return (EXIT_FAILURE);
	return (0);
}

//Sets standard values for table.
void	set_standard(t_table *t)
{
	t->die_time = 0;
	t->eat_time = 0;
	t->fork = NULL;
	t->mandatory_eat = -1;
	t->nr_philos = 0;
	t->pass = 1;
	t->philosopher = NULL;
	t->sleep_time = 0;
	t->terminate = 0;
}

//Creates a new table and sets variables.
t_table	*init_table(char *number, char *die, char *eat, char *sleep)
{
	t_table	*new;

	new = NULL;
	new = malloc(sizeof(t_table));
	if (new == NULL)
		return (error(2), NULL);
	set_standard(new);
	new->nr_philos = atoi_check(number);
	new->die_time = atoi_check(die);
	new->eat_time = atoi_check(eat);
	new->sleep_time = atoi_check(sleep);
	if (new->nr_philos < 1 || new->nr_philos > 200 || new->die_time < 1 \
		|| new->eat_time < 1 || new->sleep_time < 1)
		return (error(1), free_table(new), NULL);
	new->fork = malloc(sizeof(pthread_mutex_t) * new->nr_philos);
	if (new->fork == NULL)
		return (error(2), free_table(new), NULL);
	if (pthread_mutex_init(&new->term_mutex, NULL) != 0 \
		|| pthread_mutex_init(&new->output_mutex, NULL) != 0 \
		|| pthread_mutex_init(&new->pass_mutex, NULL))
		return (error(3), terminate(new), NULL);
	return (new);
}
