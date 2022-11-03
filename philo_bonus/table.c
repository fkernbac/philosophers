/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkernbac <fkernbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 14:40:44 by fkernbac          #+#    #+#             */
/*   Updated: 2022/10/26 17:55:29 by fkernbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//Starts every second philosopher beginning at index start.
void	start_half(t_table *t, int start)
{
	int	pid;

	pid = 2;
	while (start < t->nr_philos && pid > 0)
	{
		pid = fork();
		t->philosopher[start].pid = pid;
		if (pid == 0)
			philo_start(&t->philosopher[start]);
		else if (pid < 0)
			error(3, t);
		start += 2;
	}
}

//Creates philosopher structs.
void	create_philosophers(t_table *table)
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
		i++;
	}
}

//Sets start time and starts even and uneven philosophers.
void	fill_table(t_table *table)
{
	table->philosopher = malloc \
		(sizeof(t_philo) * table->nr_philos);
	if (table->philosopher == NULL)
		error(2, table);
	gettimeofday(&table->start_time, NULL);
	create_philosophers(table);
	start_half(table, 0);
	millisleep(table->eat_time);
	start_half(table, 1);
}

//Creates a new table.
t_table	*init_table(char *number, char *die, char *eat, char *sleep)
{
	t_table	*new;

	new = malloc(sizeof(t_table));
	if (new == NULL)
		error(2, NULL);
	memset(new, 0, sizeof(t_table));
	new->nr_philos = atoi_check(number, new);
	if (new->nr_philos <= 0 || new->nr_philos > 200)
		error(1, new);
	new->die_time = atoi_check(die, new);
	new->eat_time = atoi_check(eat, new);
	new->sleep_time = atoi_check(sleep, new);
	new->shutdown = 0;
	new->terminate = sem_open("/sem_term", O_CREAT | O_EXCL, S_IRWXU, 0);
	new->full = sem_open("/sem_full", O_CREAT | O_EXCL, S_IRWXU, 0);
	new->forks = sem_open("/sem_forks", O_CREAT | O_EXCL, S_IRWXU, \
		new->nr_philos);
	new->shutdown_mutex = sem_open("/sem_mutex", O_CREAT | O_EXCL, S_IRWXU, 1);
	new->output_mutex = sem_open("/sem_print", O_CREAT | O_EXCL, S_IRWXU, 1);
	if (new->terminate == SEM_FAILED || new->full == SEM_FAILED \
		|| new->forks == SEM_FAILED || new->shutdown_mutex == SEM_FAILED \
		|| new->output_mutex == SEM_FAILED)
		error(4, new);
	return (new);
}
