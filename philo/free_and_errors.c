/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_and_errors.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 16:43:56 by gtretiak          #+#    #+#             */
/*   Updated: 2025/07/09 15:12:33 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*write_error(char *msg)
{
	short	i;

	i = -1;
	while (msg[++i])
		write(2, &msg[i], 1);
	return ((void *)1);
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
		mutex_handler(&cafe->forks[i].lock, DESTROY);
		if (--num == 0)
			break ;
		mutex_handler(&cafe->philos[i].lock, DESTROY);
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
				free(cafe->forks);
			free(cafe->philos);
		}
		free(cafe->table);
	}
}

static void	detaching_threads(int num, int philo_nbr, t_data *cafe)
{
	int	i;

	i = -1;
	if (num == philo_nbr)
	{
		if (pthread_detach(cafe->waiter))
			write_error(THREAD);
	}
	while (++i <= num)
	{
		if (pthread_detach(cafe->philos[i].th))
			write_error(THREAD);
	}
}

int	cleanup(int code, int num, char *msg, t_data *cafe)
{
	long	philo_nbr;

	philo_nbr = cafe->table->n_philos_reserved;
	if (msg)
		write_error(msg);
	if (code == 22)
		freeing(num, cafe);
	else if (code == 35)
	{
		destroying_mutexes(num, cafe);
		freeing(3, cafe);
	}
	else if (code == 11)
	{
		destroying_mutexes(philo_nbr * 2 + 2, cafe);
		detaching_threads(num, philo_nbr, cafe);
		freeing(3, cafe);
	}
	else if (code == 0)
	{
		destroying_mutexes(philo_nbr * 2 + 2, cafe);
		detaching_threads(philo_nbr, philo_nbr, cafe);
		freeing(3, cafe);
	}
	return (code);
}
