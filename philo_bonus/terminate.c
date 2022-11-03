/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminate.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkernbac <fkernbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/10 18:46:58 by fkernbac          #+#    #+#             */
/*   Updated: 2022/10/26 20:49:07 by fkernbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//Closes all semaphores.
void	close_semaphores(t_table *t)
{
	sem_unlink("/sem_term");
	sem_unlink("/sem_forks");
	sem_unlink("/sem_full");
	sem_unlink("/sem_mutex");
	sem_unlink("/sem_print");
	if (t == NULL)
		return ;
	sem_close(t->terminate);
	sem_close(t->full);
	sem_close(t->forks);
	sem_close(t->shutdown_mutex);
	sem_close(t->output_mutex);
}

//Frees table if allocated.
void	free_table(t_table *t)
{
	if (t && t->philosopher)
		free(t->philosopher);
	if (t)
		free(t);
}

//Returns error message, frees semaphores and memory.
int	error(int type, t_table *t)
{
	if (type == 1)
		printf("Parameter error\n");
	if (type == 2)
		printf("Malloc error\n");
	if (type == 3)
		printf("Process creation error\n");
	if (type == 4)
		printf("Semaphore error\n");
	if (type == 5)
		printf("Thread creation error\n");
	close_semaphores(t);
	free_table(t);
	exit(EXIT_FAILURE);
}

//Frees resources, closes semaphores and waits for thread to join.
int	terminate_child(t_table *t, int forks)
{
	pthread_join(t->term_check, NULL);
	pthread_join(t->starve_check, NULL);
	while (forks > 0)
	{
		sem_post(t->forks);
		forks--;
	}
	close_semaphores(t);
	free_table(t);
	exit(EXIT_SUCCESS);
}

//Closes and unlinks all semaphores and frees table.
int	terminate_parent(t_table *t)
{
	close_semaphores(t);
	free_table(t);
	exit(EXIT_SUCCESS);
}
