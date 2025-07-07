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
		return (-2);
	ret = *value;
	if (mutex_handler(mutex, UNLOCK))
		return (-2);
	return (ret);
}

int	set_long(pthread_mutex_t *mutex, long *var, long value)
{
	if (mutex_handler(mutex, LOCK))
		return (1);
	*var = value;
	if (mutex_handler(mutex, UNLOCK))
		return (1);
	return (0);
}

int	set_bool(pthread_mutex_t *mutex, bool *var, bool value)
{
	if (mutex_handler(mutex, LOCK))
		return (1);
	*var = value;
	if (mutex_handler(mutex, UNLOCK))
		return (1);
	return (0);
}

long	increase_long(pthread_mutex_t *mutex, long *value)
{
	if (mutex_handler(mutex, LOCK))
		return (2);
	(*value)++;
	if (mutex_handler(mutex, UNLOCK))
		return (2);
	return (0);
}
