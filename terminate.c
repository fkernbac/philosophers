/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminate.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkernbac <fkernbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/10 18:46:58 by fkernbac          #+#    #+#             */
/*   Updated: 2022/10/31 15:15:26 by fkernbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//Destroys every mutex of table.
void	mutex_cleanup(t_table *table)
{
	int	i;

	i = -1;
	if (table == NULL)
		return ;
	while (++i < table->nr_philos)
	{
		if (table->fork)
			pthread_mutex_destroy(&table->fork[i]);
		if (table->philosopher)
			pthread_mutex_destroy(&table->philosopher[i].meal_nr_mutex);
	}
	pthread_mutex_destroy(&table->term_mutex);
	pthread_mutex_destroy(&table->output_mutex);
	pthread_mutex_destroy(&table->pass_mutex);
}

//Frees allocated memory of table.
void	free_table(t_table *table)
{
	if (table && table->philosopher)
		free(table->philosopher);
	if (table && table->fork)
		free(table->fork);
	if (table)
		free(table);
}

//Prints error to standard output and returns 1.
int	error(int type)
{
	if (type == 1)
		printf("Parameter error\n");
	if (type == 2)
		printf("Malloc error\n");
	if (type == 3)
		printf("Thread creation error\n");
	if (type == 4)
		printf("Mutex error\n");
	return (EXIT_FAILURE);
}

//Destroys mutexes and frees table.
void	terminate(t_table *table)
{
	mutex_cleanup(table);
	free_table(table);
}

//Returns -1 for use in cleanup unlocks.
int	unlock(pthread_mutex_t *m1, pthread_mutex_t *m2)
{
	if (m1)
		pthread_mutex_unlock(m1);
	if (m2)
		pthread_mutex_unlock(m2);
	return (-1);
}
