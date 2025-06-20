/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutexes.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 15:49:58 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/20 16:50:26 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "preliminary.h"

int	mails = 0;
pthread_mutex_t	mutex;

static void	*start_routine(void *arg)
{
	(void)arg;
	for (int i = 0; i < 1000000; i++)
	{
		pthread_mutex_lock(&mutex); // Now it's locked for all the threads
		mails++;
		pthread_mutex_unlock(&mutex);
	}
	return (NULL);
}

void	mutexes(void)
{
	pthread_t	t1, t2, t3, t4;
	
	pthread_mutex_init(&mutex, NULL);
	pthread_create(&t1, NULL, start_routine, NULL);
	pthread_create(&t2, NULL, start_routine, NULL);
	pthread_create(&t3, NULL, start_routine, NULL);
	pthread_create(&t4, NULL, start_routine, NULL);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);
	pthread_join(t4, NULL);
	pthread_mutex_destroy(&mutex);
	printf("%d\n", mails);
}
