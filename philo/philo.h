/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkernbac <fkernbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 21:05:08 by fkernbac          #+#    #+#             */
/*   Updated: 2022/11/04 15:11:15 by fkernbac         ###   ########.fr       */
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
	int				nr_philos;
	int				die_time;
	int				eat_time;
	int				sleep_time;
	int				mandatory_eat;
	int				terminate;
	pthread_mutex_t	term_mutex;
	int				pass;
	pthread_mutex_t	pass_mutex;
	pthread_mutex_t	output_mutex;
	struct timeval	start_time;
	pthread_mutex_t	*fork;
	struct s_philo	*philosopher;
}					t_table;

typedef struct s_philo
{
	pthread_t		id;
	int				number;
	int				index;
	int				meal_nr;
	pthread_mutex_t	meal_nr_mutex;
	struct timeval	meal_time;
	pthread_mutex_t	meal_time_mutex;
	t_table			*table;

}					t_philo;

//terminate
int		error(int type);
void	terminate(t_table *table);
void	free_table(t_table *table);
int		unlock(pthread_mutex_t *m1, pthread_mutex_t *m2);
//table
t_table	*init_table(int argc, char **argv);
int		fill_table(t_table *table);
//philosopher
void	*philo_start(void *arg);
//time
void	millisleep(int milliseconds);
int		time_since(struct timeval start);
//checks
int		check_termination(t_table *table);
int		set_death(t_table *table);
int		full_already(t_table *table);
//input output
int		atoi_check(const char *s);
int		print_action( t_philo *philo, char action);

#endif
