/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_in_loop.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 16:20:32 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/20 17:15:32 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "preliminary.h"

int	x = 0;

static void	*start_routine(void *arg)
{
	(void)arg;

	for (int i = 0; i < 10000; i++)
	{
		pthread_mutex_lock(&mutex);
		x++;
		pthread_mutex_unlock(&mutex);
	}
	printf("%d\n", x);
	return (NULL);
}

void	threads_in_loop(void)
{
	pthread_t	th[4];
	pthread_mutex_init(&mutex, NULL);
	for (size_t i = 0; i < (sizeof(th) / sizeof(pthread_t)); i++)
	{
		if (pthread_create(&th[i], NULL, start_routine, NULL) != 0)
			exit(EXIT_FAILURE);
		printf("Thread %zu has started.\n", i + 1);
	}
	for (size_t i = 0; i < (sizeof(th) / sizeof(pthread_t)); i++)
	{
		if (pthread_join(th[i], NULL) != 0)
			exit(EXIT_FAILURE);
		printf("Thread %zu has finished.\n", i + 1);
	}
	pthread_mutex_destroy(&mutex);
}
