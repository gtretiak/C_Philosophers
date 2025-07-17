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
	if (ft_strncmp(msg, ARGS, 43))
		handle_mtx(&table->print_lock, LOCK, table);
	i = -1;
	while (msg[++i])
	{
		if (write(2, &msg[i], 1) < 0)
			return ((void *)1);
	}
	if (ft_strncmp(msg, ARGS, 43))
		handle_mtx(&table->print_lock, UNLOCK, table);
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
	if (num > philo_nbr) // this is for the waiter-thread
	{
		--num;
		if (pthread_detach(cafe->waiter))
			write_error(THREAD, cafe->table); // even if error, we should try to detach others
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

	if (code != 100) // otherwise table is never malloced -> segfault!
		philo_nbr = cafe->table->n_philos_reserved; // we don´t need a mutex, since this is the only place where we fetch the data from it
	if (msg && ft_strncmp(msg, ARGS, 43)) // Again, if it´s the ARGS error, table is never malloced
		write_error(msg, cafe->table);
	else if (msg) // Instead, for ARGS, we pass NULL
		write_error(msg, NULL);
	if (code == 22)
		freeing(num, cafe); // num serves as an indicator of how many memory blocks were allocated and should be freed
	else if (code == 35)
		destroying_mutexes(num, cafe); // we use num as an indicator of how many mutexes we should destroy
	else if (code == 11)
	{
		destroying_mutexes(philo_nbr * 2 + 2, cafe); // here we know that we previously successfully created mutexes for every philo, every fork + for table and printing
		detaching_threads(num, philo_nbr, cafe); // we use num as an indicator of how many threads were created and should be detached
	}
	else if (code == 0)
		destroying_mutexes(philo_nbr * 2 + 2, cafe);
	if (code == 35 || code == 11 || code == 0)
		freeing(3, cafe); // with those codes we can be absolutely sure that all three memory blocks we use in the program were allocated
	return (code);
}
