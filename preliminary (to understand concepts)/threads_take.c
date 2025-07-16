/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_take.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 17:43:35 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/20 18:20:32 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "preliminary.h"

int	primes[10] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };

static void	*routine(void *arg)
{
	//sleep(1);// i is equal 0 again now (if we use the same i for 2nd loop)
	// or index is 10 now (with same garbage value)
	// So it's better to allocate different i for every thread
 	int	value;
	value = *(int *)arg;
	printf("Value %d  has been passed with the address:%p\n", value, &value);
//	return (value); instead of return it, we need to free it
//	free(arg);
	return (NULL);
}

void	threads_take(void)
{
	srand(time(NULL));
	pthread_t	th[10];
	for (size_t i = 0; i < (sizeof(th) / sizeof(pthread_t)); i++)
	{
//		int	*a = malloc(4);
//		*a = i;
//		BUT since primes is global we can pass the addresses of its members directly
		if (pthread_create(&th[i], NULL, routine, &primes[i]) != 0) // Tricky part here is that
								    // referenced value might changed
								    // while routine
								    // 2 or more threads might have i=1
								    // So let's use allocated a
			exit(EXIT_FAILURE);
	}
	for (size_t i = 0; i < (sizeof(th) / sizeof(pthread_t)); i++)
	{
		if (pthread_join(th[i], NULL) != 0)
			exit(EXIT_FAILURE);
	}
}
