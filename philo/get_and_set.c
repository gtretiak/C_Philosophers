/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_and_set.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 17:43:43 by gtretiak          #+#    #+#             */
/*   Updated: 2025/07/02 18:11:30 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_long(pthread_mutex_t *mutex, long *value)
{
	long	ret;

	if (*value == -777 || mutex_handler(mutex, LOCK))
		ret = -2;
	else
		ret = *value;
	if (mutex_handler(mutex, UNLOCK))
		ret = -2;
	return (ret);
}

int	set_long(pthread_mutex_t *mutex, long *var, long value)
{
	int	ret;

	if (mutex_handler(mutex, LOCK))
		ret = 1;
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
		ret = 1;
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
		ret = 2;
	else
	{
		(*value)++;
		ret = 0;
	}
	if (mutex_handler(mutex, UNLOCK))
		ret = 2;
	return (ret);
}
