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

void	write_error(char *msg)
{
	short	i;

	i = -1;
	while (msg[++i])
		write(2, &msg[i], 1);
}

static void	destroying_mutexes(int num, t_data *cafe)
{
	int	i;

	i = -1;
	if (num-- >= 2)
		mutex_handler(&cafe->table->print_lock, DESTROY);
	if (num-- >= 1)
		mutex_handler(&cafe->table->lock, DESTROY);
	while (num && ++i <= num)
	{
		mutex_handler(&cafe->all_forks[i].lock, DESTROY);
                if (--num == 0)
			break ;
                mutex_handler(&cafe->all_philos[i].lock, DESTROY);
		--num;
	}
}

static void	freeing(int num, t_data *cafe)
{
	if (num >= 1)
	{
		if (num >= 2)
		{
			if (num >= 3)
				free(cafe->all_forks);
			free(cafe->all_philos);
		}
		free(cafe->table);
	}
}

static void	detaching_threads(int num, int philo_nbr, t_data *cafe)
{
	int	i;

	i = -1;
	if (num == philo_nbr)
		pthread_detach(cafe->waiter);
	while (++i <= num)
		pthread_detach(cafe->all_philos[i].philo_acting);
}

int	handle_error(int code, int num, char *msg, t_data *cafe)
{
	long	philo_nbr;

	if (msg)
		write_error(msg);
	// 14 gettimeofday
	if (code == 22) //invalid
		freeing(num, cafe);	
	else if (code == 35) //mutexes
	{
		destroying_mutexes(num, cafe);
		freeing(3, cafe);
	}
		else if (code == 11) //threads
		{
			philo_nbr = get_long(&cafe->table->lock, &cafe->table->n_philos);
			if (philo_nbr == -2)
				return (handle_error(35, 3, MUTEX, cafe))
			destroying_mutexes(philo_nbr * 2 + 2, cafe);
			detaching_threads(num, philo_nbr, cafe);
		}
		//if (cleanup(cafe, code))
		//	return (code + handle_error(cafe, 1, MUTEX));
	}
	return(code);
}

int	cleanup(t_data *cafe, int code)
{
	int	i;

	i = -1;
	if (code >= 2)
	{
		if (code >= 3)
		{
			if (code >= 4)
			{
				while (++i < cafe->table->n_philos)
				{
					if (mutex_handler(&cafe->all_forks[i].lock, DESTROY) || mutex_handler(&cafe->all_philos[i].lock, DESTROY))
						return (1);
				}
				if (mutex_handler(&cafe->table->lock, DESTROY) || mutex_handler(&cafe->table->print_lock, DESTROY))
					return (1);
			}
		}
	}
	return (0);
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
