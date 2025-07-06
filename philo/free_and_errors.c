/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_and_errors.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 16:43:56 by gtretiak          #+#    #+#             */
/*   Updated: 2025/07/02 19:08:23 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	handle_error(t_data *cafe, int code, char *msg)
{
	short	i;

	i = -1;
	while (msg[++i])
		write(2, &msg[i], 1);
	if (code > 1)
		cleanup(cafe, code);
	return(code);
}

void	cleanup(t_data *cafe, int code)
{
	int	i;

	i = -1;
	if (code >= 2)
	{
		if (code >= 3)
		{
			while (++i < cafe->table->n_philos)
				pthread_join(cafe->all_philos[i].philo_acting, NULL);
			pthread_join(cafe->waiter, NULL);
			if (code == 4)
			{
				if (code == 5)
				{	while (++i < cafe->table->n_philos)
						pthread_join(cafe->all_philos[i].philo_acting, NULL);
					pthread_join(cafe->waiter, NULL);
					i = -1;
				}
				while (++i < cafe->table->n_philos)
				{
					mutex_handler(&cafe->all_forks[i].mtx, DESTROY);
					mutex_handler(&cafe->all_philos[i].philo_lock, DESTROY);
				}
				mutex_handler(&cafe->table->lock, DESTROY);
				mutex_handler(&cafe->table->print_lock, DESTROY);
				mutex_handler(&cafe->common_lock, DESTROY);
				free(cafe->all_forks);
			}
			free(cafe->all_philos);
		}
		free(cafe->table);
	}
}

int	mutex_handler(pthread_mutex_t *mutex, t_code code)
{
	if (code == INIT)
		return (pthread_mutex_init(mutex, NULL));
	else if (code == LOCK)
		return (pthread_mutex_lock(mutex));
	else if (code == UNLOCK)
		return (pthread_mutex_unlock(mutex));
	else if (code == DESTROY)
		return (pthread_mutex_destroy(mutex));
	return (1);
}
