/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   waiter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 18:50:28 by gtretiak          #+#    #+#             */
/*   Updated: 2025/07/10 18:51:50 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	wait_all_threads(t_table *table)
{
	int	i;

	while (1)
	{
		i = all_running(table);
		if (i == 1)
			return (0);
		else if (i < 0)
			return (1);
	}
	return (0);
}

static int	supervise_loop(t_data *cafe, long iter, int *i)
{
	long	temp;

	while (1)
	{
		*i = -1;
		while (++(*i) < iter
			&& get_long(&cafe->table->lock,
				&cafe->table->dinner_is_over, cafe->table) == 0
			&& get_long(&cafe->table->lock,
				&cafe->table->n_full, cafe->table) < iter)
		{
			temp = is_philo_dead(&cafe->philos[*i]);
			if (temp != 0)
				return (temp < 0);
			temp = is_everyone_full(&cafe->philos[*i], iter);
			if (temp < 0)
				return (1);
			else if (temp == 1)
				return (0);
		}
		if (sleeping(2e2, get_time(US, cafe->table), cafe->table))
			return (1);
	}
	return (0);
}

void	*serving(void *arg)
{
	t_data	*cafe;
	long	iter;
	int		i;

	cafe = (t_data *)arg;
	if (wait_all_threads(cafe->table))
		return ((void *)1);
	iter = get_long(&cafe->table->lock, &cafe->table->n_philos, cafe->table);
	if (iter == -2)
		return ((void *)1);
	if (supervise_loop(cafe, iter, &i))
		return ((void *)1);
	return ((void *)0);
}
