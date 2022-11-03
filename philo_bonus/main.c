/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkernbac <fkernbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 21:02:55 by fkernbac          #+#    #+#             */
/*   Updated: 2022/10/26 17:42:09 by fkernbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//Waits for every child process to terminate.
//Prints death message depending on child exit status.
int	wait_for_termination(t_table *table)
{
	int	i;

	i = 0;
	while (i++ < table->nr_philos)
		waitpid(-1, NULL, 0);
	return (0);
}

//Thread waits for meal semaphore and increments termination semaphore.
void	*wait_for_meals(t_table *t)
{
	int		i;

	i = 0;
	if (t->mandatory_eat < 0)
		return (NULL);
	while (i++ < t->nr_philos)
		sem_wait(t->full);
	while (i-- >= 0)
		sem_post(t->terminate);
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_table		*t;

	sem_unlink("/sem_term");
	sem_unlink("/sem_forks");
	sem_unlink("/sem_full");
	sem_unlink("/sem_mutex");
	sem_unlink("/sem_print");
	t = NULL;
	if (argc < 5 || argc > 6)
		return (error(1, NULL));
	t = init_table(argv[1], argv[2], argv[3], argv[4]);
	if (t == NULL)
		return (EXIT_FAILURE);
	if (argc == 6)
		t->mandatory_eat = atoi_check(argv[5], t);
	else
		t->mandatory_eat = -1;
	fill_table(t);
	wait_for_meals(t);
	wait_for_termination(t);
	terminate_parent(t);
}
