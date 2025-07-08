/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   settings.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 17:51:08 by gtretiak          #+#    #+#             */
/*   Updated: 2025/07/02 18:53:28 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	assign_forks(t_philo *philo, t_fork *forks, int i, int n_philo)
{
	int	left_fork;
	int	right_fork;

	left_fork = i;
	if (philo->position != 0)
		right_fork = (i + 1) % n_philo;
	else
		right_fork = n_philo - 1;
	philo->first_fork = &forks[right_fork];
	philo->first_fork->index = right_fork;
	philo->second_fork = &forks[left_fork];
	philo->second_fork->index = left_fork;
	if (philo->position % 2 == 0)
	{
		philo->first_fork = &forks[left_fork];
		philo->first_fork->index = left_fork;
		philo->second_fork = &forks[right_fork];
		philo->second_fork->index = right_fork;
	}
/*	printf("The %lu philo should always take "
		"%d (first) fork and %d (second) fork\n",
		philo->position, philo->first_fork->index, philo->second_fork->index);*/
}

static void	setup_private(t_data *cafe, int i)
{
	cafe->all_philos[i].position = i + 1;
	cafe->all_philos[i].rip = 0;
	cafe->all_philos[i].meals_eaten = 0;
	cafe->all_philos[i].full = 0;
	cafe->all_philos[i].table = cafe->table;
	cafe->all_forks[i].index = i;
	cafe->all_forks[i].taken = false;
}

static int	setup_common(t_data *cafe)
{
	if (mutex_handler(&cafe->table->lock, INIT)
		|| mutex_handler(&cafe->table->print_lock, INIT))
		return (1);
	cafe->table->dinner_is_over = 0;
	cafe->table->all_ready = 0;
	cafe->table->running_threads = 0;
	cafe->table->n_full = 0;
	return (0);
}

int	init(t_data *cafe)
{
	int	i;

	i = -1;
	cafe->all_philos = malloc(cafe->table->n_philos * sizeof(t_philo));
	if (cafe->all_philos == NULL)
		return (handle_error(cafe, 2, MALLOC));
	cafe->all_forks = malloc(cafe->table->n_philos * sizeof(t_fork));
	if (cafe->all_forks == NULL)
		return (handle_error(cafe, 3, MALLOC));
	if (setup_common(cafe))
		return (handle_error(cafe, 4, MUTEX));
	while (++i < cafe->table->n_philos)
	{
		setup_private(cafe, i);
		if (mutex_handler(&cafe->all_forks[i].lock, INIT))
			return (handle_error(cafe, 4, MUTEX));
		if (mutex_handler(&cafe->all_philos[i].lock, INIT))
			return (handle_error(cafe, 4, MUTEX));
		assign_forks(&cafe->all_philos[i], cafe->all_forks,
			i, cafe->table->n_philos);
	}
	return (0);
}
