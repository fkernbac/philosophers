/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkernbac <fkernbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 14:40:44 by fkernbac          #+#    #+#             */
/*   Updated: 2022/11/05 21:52:26 by fkernbac         ###   ########.fr       */
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
	millisleep(5);
	start_half(table, 1);
}

//Sets standard values for table.
void	create_semaphores(t_table *table)
{
	table->terminate = sem_open("/sem_term", O_CREAT | O_EXCL, S_IRWXU, 0);
	table->full = sem_open("/sem_full", O_CREAT | O_EXCL, S_IRWXU, 0);
	table->forks = \
		sem_open("/sem_forks", O_CREAT | O_EXCL, S_IRWXU, table->nr_philos);
	table->shutdown_mutex = \
		sem_open("/sem_mutex", O_CREAT | O_EXCL, S_IRWXU, 1);
	table->output_mutex = sem_open("/sem_print", O_CREAT | O_EXCL, S_IRWXU, 1);
	if (table->terminate == SEM_FAILED || table->full == SEM_FAILED \
		|| table->forks == SEM_FAILED || table->shutdown_mutex == SEM_FAILED \
		|| table->output_mutex == SEM_FAILED)
		error(4, table);
}

//Creates a new table.
t_table	*init_table(int argc, char **argv)
{
	t_table	*new;

	new = malloc(sizeof(t_table));
	if (new == NULL)
		error(2, NULL);
	memset(new, 0, sizeof(t_table));
	new->nr_philos = atoi_check(argv[1], new);
	new->die_time = atoi_check(argv[2], new);
	new->eat_time = atoi_check(argv[3], new);
	new->sleep_time = atoi_check(argv[4], new);
	new->mandatory_eat = -1;
	if (argc == 6)
		new->mandatory_eat = atoi_check(argv[5], new);
	if (new->nr_philos <= 0 || new->nr_philos > 200 || new->die_time < 1 \
		|| new->eat_time < 1 || new->sleep_time < 1)
		error(1, new);
	new->shutdown = 0;
	create_semaphores(new);
	return (new);
}
