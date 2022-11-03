/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkernbac <fkernbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 21:05:08 by fkernbac          #+#    #+#             */
/*   Updated: 2022/11/03 18:08:39 by fkernbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <pthread.h>	//pthread
# include <stdio.h>		//printf
# include <stdlib.h>	//malloc
# include <limits.h>	//INT_MAX
# include <sys/time.h>	//gettimeofday
# include <sys/wait.h>	//waitpid
# include <sys/stat.h>	//S_IRWXU
# include <fcntl.h>		//O_CREAT
# include <unistd.h>	//usleep
# include <semaphore.h>	//sem_t
# include <string.h>	//memset
// # define malloc(x) NULL

typedef struct s_table
{
	int				nr_philos;
	int				die_time;
	int				eat_time;
	int				sleep_time;
	int				mandatory_eat;
	struct timeval	start_time;
	int				shutdown;
	sem_t			*shutdown_mutex;
	sem_t			*output_mutex;
	sem_t			*terminate;
	sem_t			*full;
	sem_t			*forks;
	pthread_t		term_check;
	pthread_t		starve_check;
	struct s_philo	*philosopher;
}					t_table;

typedef struct s_philo
{
	int				pid;
	int				number;
	int				index;
	int				meal_nr;
	struct timeval	meal_time;
	t_table			*table;
}					t_philo;

//terminate
int		terminate_parent(t_table *table);
int		terminate_child(t_table *t, int forks);
int		error(int type, t_table *table);
//table
t_table	*init_table(int argc, char **argv);
void	fill_table(t_table *table);
//philosopher
void	philo_start(void *arg);
//time
void	millisleep(int milliseconds);
int		time_since(struct timeval start);
//checks
int		check(t_table *table);
int		am_i_dead(t_philo *philo);
int		am_i_full(t_philo *philo);
//input output
int		print_action(t_philo *philo, char action);
int		print_action_timed(t_philo *philo, char action, int time);
int		atoi_check(const char *s, t_table *table);

#endif
