/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_and_set.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 17:43:43 by gtretiak          #+#    #+#             */
/*   Updated: 2025/07/09 13:52:46 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	mutex_handler(pthread_mutex_t *mutex, t_code code)
{
	int	ret;

	ret = 1;
	if (code == INIT)
		ret = pthread_mutex_init(mutex, NULL);
	else if (code == LOCK)
		ret = pthread_mutex_lock(mutex);
	else if (code == UNLOCK)
		ret = pthread_mutex_unlock(mutex);
	else if (code == DESTROY)
		ret = pthread_mutex_destroy(mutex);
	if (ret)
		write_error(MUTEX);
	return (ret);
}

long	get_long(pthread_mutex_t *mutex, long *value)
{
	long	ret;

	if (*value == -666 || mutex_handler(mutex, LOCK))
		return (-2);
	else
		ret = *value;
	if (mutex_handler(mutex, UNLOCK))
		ret = -2;
	return (ret);
}

int	set_long(pthread_mutex_t *mutex, long *var, long value)
{
	int	ret;

	if (value == -666 || mutex_handler(mutex, LOCK))
		return (1);
	else
	{
		*var = value;
		ret = 0;
	}
	if (mutex_handler(mutex, UNLOCK))
		ret = 1;
	return (ret);
}

int	set_bool(pthread_mutex_t *mutex, bool *var, bool value)
{
	int	ret;

	if (mutex_handler(mutex, LOCK))
		return (1);
	else
	{
		*var = value;
		ret = 0;
	}
	if (mutex_handler(mutex, UNLOCK))
		ret = 1;
	return (ret);
}

long	increase_long(pthread_mutex_t *mutex, long *value)
{
	long	ret;

	if (mutex_handler(mutex, LOCK))
		return (2);
	else
	{
		(*value)++;
		ret = 0;
	}
	if (mutex_handler(mutex, UNLOCK))
		ret = 2;
	return (ret);
}
