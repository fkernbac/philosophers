/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkernbac <fkernbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 21:05:08 by fkernbac          #+#    #+#             */
/*   Updated: 2022/09/13 19:44:53 by fkernbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_table
{
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				mandatory_eat;
	struct timeval	start_time;
	pthread_mutex_t	*fork;
	struct s_philo	*philosopher;
}					t_table;

typedef struct s_philo
{
	pthread_t		id;
	int				number;
	int				index;
	int				alive;
	int				meals;
	struct timeval	last_meal;
	t_table			*table;
}					t_philo;

int		atoi_check(const char *s, t_table *table);
int		terminate(t_table *table);
int		error(int type, t_table *table);
void	free_table(t_table *table);
//philosopher operations
void	philo_eat(t_table *table, int index);
void	philo_sleep(t_table *table, int index);
void	philo_think(t_table *table, int index);

#endif
