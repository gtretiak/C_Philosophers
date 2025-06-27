/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 15:48:53 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/27 19:14:08 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	run_simulation(t_data *table)
{
	int	i;

	i = -1;
	if (table->n_philos == 1)
		run_one(); // TODO
	else
	{
		while (++i < table->n_philos)
			pthread_create(&table->all_philos[i],
				dinner, table->all_philos[i]);
		i = -1;
		while (++i < table->n_philos)
			pthread_join(&table->all_philos[i], NULL;
	}
}
void	*dinner(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;


	return (NULL);
}

void	*serving(void *arg)
{
	t_data	*table;

	table = (t_data *)arg;
	if (table->the_end) // check and displaying within 10ms of actual death
		//finishing
}
