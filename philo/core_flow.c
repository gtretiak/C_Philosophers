/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_flow.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 18:59:15 by gtretiak          #+#    #+#             */
/*   Updated: 2025/07/10 18:59:17 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	thread_creation(t_data *cafe, long *philo_nbr, int *i)
{
	if (*philo_nbr == 1)
	{
		*i = 1;
		if (pthread_create(&cafe->philos[0].th, NULL,
				run_alone, &cafe->philos[0]))
			return (cleanup(11, 0, THREAD, cafe));
	}
	else
	{
		while (++(*i) < *philo_nbr)
		{
			if (pthread_create(&cafe->philos[*i].th, NULL,
					dinner, &cafe->philos[*i]))
				return (cleanup(11, *i, THREAD, cafe));
		}
	}
	if (pthread_create(&cafe->waiter, NULL, serving, cafe))
		return (cleanup(11, *i, THREAD, cafe));
	return (0);
}

static int	thread_joining(t_data *cafe, long philo_nbr, int i)
{
	void		*ret;

	if (pthread_join(cafe->waiter, (void *)&ret))
		return (cleanup(11, i, THREAD, cafe));
	else if (ret != (void *)0)
		return (cleanup(11, i, NULL, cafe));
	if (set_long(&cafe->table->lock,
			&cafe->table->dinner_is_over, 1, cafe->table))
		return (cleanup(11, i, THREAD, cafe));
	if (philo_nbr == 1)
		i = 1;
	while (--i >= 0)
	{
		if (pthread_join(cafe->philos[i].th, (void *)&ret))
			return (cleanup(11, i, THREAD, cafe));
		else if (ret != (void *)0)
			return (cleanup(11, i, NULL, cafe));
	}
	return (0);
}

int	run_simulation(t_data *cafe)
{
	int			i;
	int			ret;
	long		philo_nbr;

	i = -1;
	philo_nbr = get_long(&cafe->table->lock,
			&cafe->table->n_philos, cafe->table);
	if (philo_nbr == -2)
		return (cleanup(35, philo_nbr * 2 + 2, NULL, cafe));
	ret = thread_creation(cafe, &philo_nbr, &i);
	if (ret)
		return (ret);
	if (set_long(&cafe->table->lock, &cafe->table->t_start,
			get_time(MS, cafe->table), cafe->table)
		|| set_long(&cafe->table->lock,
			&cafe->table->all_ready, 1, cafe->table))
		return (cleanup(11, i + 1, NULL, cafe));
	ret = thread_joining(cafe, philo_nbr, i);
	if (ret)
		return (ret);
	return (0);
}
