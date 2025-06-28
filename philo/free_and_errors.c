/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_and_errors.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 16:43:56 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/28 19:11:03 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	handle_error(t_data *cafe, int code, char *msg)
{
	short	i;

	i = -1;
	while (msg[++i])
		write(code, &msg[i], 1);
	if (code > 1)
		cleanup(cafe, code);
	exit(code);
}

void	cleanup(t_data *cafe, int code)
{
	int	i;

	i = -1;
	if (code >= 2)
	{
		if (code >= 3)
		{
			if (code == 4)
			{
				while (++i < cafe->table->n_philos)
					mutex_handler(&cafe->all_forks[i].mtx, DESTROY, cafe);
			        mutex_handler(&cafe->table->lock, DESTROY, cafe);
				free(cafe->all_forks);
				thread_handler(&cafe->waiter, JOIN, NULL, NULL);
			}
			i = -1;
			thread_handler(&cafe->waiter, JOIN, NULL, NULL);
			while (++i < cafe->table->n_philos)
				thread_handler(&cafe->all_philos[i].philo_acting, JOIN, NULL, NULL);
			free(cafe->all_philos);
		}
		free(cafe->table);
	}
}

void	thread_handler(pthread_t *thread, t_opcode opcode, void *(*foo)(void *), t_data *cafe)
{
	int	status;

	status = 0;
	if (opcode == CREATE)
		status = pthread_create(thread, NULL, foo, cafe);
	else if (opcode == JOIN)
		status = pthread_join(*thread, NULL);
	if (status && cafe)
		handle_error(cafe, 4, THREAD);
}

void	mutex_handler(pthread_mutex_t *mutex, t_opcode opcode, t_data *cafe)
{
	int	status;

	status = 0;
	if (opcode == INIT)
		status = pthread_mutex_init(mutex, NULL);
	else if (opcode == LOCK)
		status = pthread_mutex_lock(mutex);
	else if (opcode == UNLOCK)
		status = pthread_mutex_unlock(mutex);
	else if (opcode == DESTROY)
		status = pthread_mutex_destroy(mutex);
	if (status && cafe)
		handle_error(cafe, 4, MUTEX);
}
