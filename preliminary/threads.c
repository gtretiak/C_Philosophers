/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 15:49:25 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/20 17:57:17 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "preliminary.h"

static void	*start_routine(void *arg)
{
	int	*x = malloc(4);

	*x = 0;
	printf("Thread with string arg (if exists): %s\n", (char *)arg);
	printf("Thread with int arg (if exists): %d\n", *(int *)arg);
	for (int i = 0; i < 10000; i++) // it's not always 10000 and 20000,
					// because of the race conditions
					// but if we tried 100000 it fails every time
					// because every time we need to read the value,
					// increment it and write the updated value.
					// Since one thread might set on pause for a while
					// after reading, it's gonna rewrite the value incorrectly
		(*x)++;
	printf("%d\n", *x);
	return (x);
}

void	threads(void)
{
	pthread_t	t1, t2;
	void	*ret;
	int	num = 666;

	if (pthread_create(&t1, NULL, start_routine, "some1") != 0)
		exit(EXIT_FAILURE);
	if (pthread_create(&t2, NULL, start_routine, &num) != 0)
		exit(EXIT_FAILURE);
	if (pthread_join(t1, &ret) != 0)
		exit(EXIT_FAILURE);
	printf("Thread returned: %d with address: %p\n", *(int *)ret, ret);
	free(ret);
	if (pthread_join(t2, &ret) != 0)
		exit(EXIT_FAILURE);
	printf("Thread returned: %d with address: %p\n", *(int *)ret, ret);
	free(ret);
}
