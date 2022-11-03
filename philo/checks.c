/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkernbac <fkernbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 14:43:01 by fkernbac          #+#    #+#             */
/*   Updated: 2022/10/31 18:17:17 by fkernbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//Checks if termination variable is set.
int	check_termination(t_table *table)
{
	int	dead;

	dead = 0;
	pthread_mutex_lock(&table->term_mutex);
	if (table->terminate == 1)
		dead = 1;
	pthread_mutex_unlock(&table->term_mutex);
	return (dead);
}

//Checks if another death has already occured, before starting termination.
int	set_death(t_table *table)
{
	int	success;

	success = 0;
	pthread_mutex_lock(&table->term_mutex);
	if (table->terminate == 1)
		success = -1;
	else
		table->terminate = 1;
	pthread_mutex_unlock(&table->term_mutex);
	return (success);
}
