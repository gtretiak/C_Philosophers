/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_and_errors.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 16:43:56 by gtretiak          #+#    #+#             */
/*   Updated: 2025/07/13 16:03:28 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*write_error(char *msg, t_table *table)
{
	short	i;

	(void)table;
	//if (ft_strncmp(msg, ARGS, 43))
	//	handle_mtx(&table->print_lock, LOCK, table);
	i = -1;
	while (msg[++i])
	{
		if (write(2, &msg[i], 1) < 0)
			return ((void *)1);
	}
	//if (ft_strncmp(msg, ARGS, 43))
	//	handle_mtx(&table->print_lock, UNLOCK, table);
	return ((void *)1);
}

static void	destroying_mutexes(int num, t_data *cafe)
{
	int	i;

	i = -1;
	if (num-- >= 2)
		handle_mtx(&cafe->table->print_lock, DESTROY, cafe->table);
	if (num-- >= 1)
		handle_mtx(&cafe->table->lock, DESTROY, cafe->table);
	while (num && ++i < num)
	{
		handle_mtx(&cafe->forks[i].lock, DESTROY, cafe->table);
		if (--num == 0)
			break ;
		handle_mtx(&cafe->philos[i].lock, DESTROY, cafe->table);
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
	if (num > philo_nbr)
	{
		--num;
		if (pthread_detach(cafe->waiter))
			write_error(THREAD, cafe->table);
	}
	while (++i < num)
	{
		if (pthread_detach(cafe->philos[i].th))
			write_error(THREAD, cafe->table);
	}
}

int	cleanup(int code, int num, char *msg, t_data *cafe)
{
	long	philo_nbr;

	if (code != 100)
		philo_nbr = cafe->table->n_philos_reserved;
	if (msg && ft_strncmp(msg, ARGS, 43))
		write_error(msg, cafe->table);
	else if (msg)
		write_error(msg, NULL);
	if (code == 22)
		freeing(num, cafe);
	else if (code == 35)
		destroying_mutexes(num, cafe);
	else if (code == 11)
	{
		destroying_mutexes(philo_nbr * 2 + 2, cafe);
		detaching_threads(num, philo_nbr, cafe);
	}
	else if (code == 0)
		destroying_mutexes(philo_nbr * 2 + 2, cafe);
	if (code == 35 || code == 11 || code == 0)
		freeing(3, cafe);
	return (code);
}
