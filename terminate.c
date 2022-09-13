/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminate.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkernbac <fkernbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/10 18:46:58 by fkernbac          #+#    #+#             */
/*   Updated: 2022/09/13 20:16:20 by fkernbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_table(t_table *table)
{
	int	i;

	i = -1;
	// while (table && table->philosopher && table->philosopher[++i])
	// 	free(table->philosopher[i]);
	if (table && table->philosopher)
		free(table->philosopher);
	if (table)
		free(table);
}

int	error(int type, t_table *table)
{
	if (type == 1)
		printf("Parameter error\n");
	if (type == 2)
		printf("Malloc error\n");
	if (type == 3)
		printf("Thread creation error\n");
	free_table(table);
	system("leaks philo");
	exit(EXIT_FAILURE);
}

int	terminate(t_table *table)
{
	free_table(table);
	system("leaks philo");
	exit(EXIT_SUCCESS);
}
