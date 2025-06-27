/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   settings.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 17:51:08 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/27 19:13:55 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
/*
void	assign_forks(t_philo *philo, t_forks *forks, int pos)
{
	philo->right_fork = &forks[pos];
	philo->left_fork = &forks[(pos + 1) % philo->position];
}*/

void	init(t_data *table)
{
	int	i;
	pthread_t	waiter;

	i = -1;
	table->the_end = false;
	table->all_philos = malloc(table->n_philos * sizeof(t_philo));//an array of philos
	if (table->all_philos == NULL)
		handle_error(MALLOC, 2);
	table->all_forks = malloc(table->n_philos * sizeof(t_fork));//an array of forks
	if (table->all_forks == NULL)
		cleanup(&table, 2, MALLOC);
	while (++i < table->n_philos)
	{
		pthread_mutex_init(&table->all_forks[i].mtx);
/*		table->all_forks[i].fork_id = i;
		table->all_philos[i].position = i + 1;
		table->all_philos[i].ready_to_eat = true;
		assign_forks(table->all_philos[i], table->all_forks, i);*/
	}
	pthread_create(&waiter, NULL, serving, table);
}
