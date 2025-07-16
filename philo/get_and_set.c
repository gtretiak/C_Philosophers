/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_and_set.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 17:43:43 by gtretiak          #+#    #+#             */
/*   Updated: 2025/07/09 17:05:30 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	handle_mtx(pthread_mutex_t *mtx, t_code code, t_table *table)
{
	int	ret;

	ret = 1;
	if (code == INIT)
		ret = pthread_mutex_init(mtx, NULL);
	else if (code == LOCK)
		ret = pthread_mutex_lock(mtx);
	else if (code == UNLOCK)
		ret = pthread_mutex_unlock(mtx);
	else if (code == DESTROY)
		ret = pthread_mutex_destroy(mtx);
	if (ret)
		write_error(MUTEX, table);
	return (ret);
}

long	get_long(pthread_mutex_t *mtx, long *value, t_table *table)
{
	long	ret;

	if (!value || handle_mtx(mtx, LOCK, table))
		return (-2);
	ret = *value;
	if (handle_mtx(mtx, UNLOCK, table))
		return (-2);
	return (ret);
}

int	set_long(pthread_mutex_t *mtx, long *var, long value, t_table *table)
{
	int	ret;

	if (value == -666 || handle_mtx(mtx, LOCK, table)) // -666 for gettimeofday return value checking
		return (1);
	else
	{
		*var = value;
		ret = 0;
	}
	if (handle_mtx(mtx, UNLOCK, table))
		ret = 1;
	return (ret);
}

int	set_bool(pthread_mutex_t *mtx, bool *var, bool value, t_table *table)
{
	int	ret;

	if (handle_mtx(mtx, LOCK, table))
		return (1);
	else
	{
		*var = value;
		ret = 0;
	}
	if (handle_mtx(mtx, UNLOCK, table))
		ret = 1;
	return (ret);
}

long	increase_long(pthread_mutex_t *mtx, long *value, t_table *table)
{
	long	ret;

	if (handle_mtx(mtx, LOCK, table))
		return (2);
	else
	{
		(*value)++;
		ret = 0;
	}
	if (handle_mtx(mtx, UNLOCK, table))
		ret = 2;
	return (ret);
}
