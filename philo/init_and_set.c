/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_and_set.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 18:44:27 by gtretiak          #+#    #+#             */
/*   Updated: 2025/07/10 18:44:29 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	assign_forks(t_philo *philo, t_fork *forks, int i, int n_philo)
{
	int	left_fork;
	int	right_fork;

	left_fork = i; // if philo position is 1 then left fork is forks[0]
	if (philo->pos != 0)
		right_fork = (i + 1) % n_philo; // right fork is forks[1]
	else
		right_fork = n_philo - 1; // or the last one in the array
	philo->fork1 = &forks[right_fork];
	philo->fork2 = &forks[left_fork];
	if (philo->pos % 2 == 0) // if position is even (in the example above not) we invert assignment
	{
		philo->fork1 = &forks[left_fork];
		philo->fork2 = &forks[right_fork];
	}
	// now we greatly reducing the chance of a deadlock, breaking the symmetry, because some philos first grab left fork, others - right one (circular wait is prevented)
}

static void	setup_private(t_data *cafe, int i)
{
	cafe->philos[i].pos = i + 1;
	cafe->philos[i].rip = 0;
	cafe->philos[i].meals_eaten = 0;
	cafe->philos[i].full = 0;
	cafe->philos[i].counted_full = 0; // to see if the fullness of a certain philo is already counted in total calculation or not
	cafe->philos[i].table = cafe->table; // we double link table-philo
	cafe->forks[i].taken = 0;
}

static int	setup_common(t_data *cafe)
{
	if (handle_mtx(&cafe->table->lock, INIT, cafe->table))
		return (cleanup(35, 0, NULL, cafe));
	if (handle_mtx(&cafe->table->print_lock, INIT, cafe->table))
		return (cleanup(35, 1, NULL, cafe));
	cafe->table->n_philos_reserved = cafe->table->n_philos; // we copy it for cleanup purposes, to use it without mutexes
	cafe->table->dinner_is_over = 0; // main simulation flag
	cafe->table->all_ready = 0; // the flag to see that all threads are ready to start simultaneously
	cafe->table->running_threads = 0; // for counting running threads before start for synchronization 
	cafe->table->n_full = 0; // number of philos that are full (to compare with total number of philos)
	return (0);
}

static int	allocation(t_data *cafe)
{
	cafe->philos = malloc(cafe->table->n_philos * sizeof(t_philo));
	if (cafe->philos == NULL)
		return (cleanup(22, 1, MALLOC, cafe));
	cafe->forks = malloc(cafe->table->n_philos * sizeof(t_fork));
	if (cafe->forks == NULL)
		return (cleanup(22, 2, MALLOC, cafe));
	return (0);
}

int	init(t_data *cafe)
{
	int	i;
	int	j;
	int	ret;

	i = -1;
	ret = allocation(cafe);
	if (ret)
		return (ret);
	ret = setup_common(cafe);
	if (ret)
		return (ret);
	j = 2; // we already initialized two mutexes here, so we should know it in case of cleaning up
	while (++i < cafe->table->n_philos)
	{
		setup_private(cafe, i);
		if (handle_mtx(&cafe->forks[i].lock, INIT, cafe->table))
			return (cleanup(35, j + i, NULL, cafe));
		j++;
		if (handle_mtx(&cafe->philos[i].lock, INIT, cafe->table))
			return (cleanup(35, j + i, NULL, cafe));
		assign_forks(&cafe->philos[i], cafe->forks,
			i, cafe->table->n_philos);
	}
	return (0);
}
