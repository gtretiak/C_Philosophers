/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_return.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 17:17:36 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/20 17:41:50 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "preliminary.h"

static void	*roll_dice(void *arg)
{
	(void)arg;
	int	*value = malloc(4);
	*value = (rand() % 6) + 1;
	printf("Value %d has been passed with the address:%p\n", *value, value);
	return (value);
}

void	threads_return(void)
{
	srand(time(NULL));
	int	*ret = NULL;
	pthread_t	th[4];
	for (size_t i = 0; i < (sizeof(th) / sizeof(pthread_t)); i++)
	{
		if (pthread_create(&th[i], NULL, roll_dice, NULL) != 0)
			exit(EXIT_FAILURE);
	}
	for (size_t i = 0; i < (sizeof(th) / sizeof(pthread_t)); i++)
	{
		if (pthread_join(th[i], (void *)&ret) != 0)
			exit(EXIT_FAILURE);
		printf("Value %d has been gained by the thread %zu with the address:%p\n",
			*(int *)ret, i + 1, ret);
		free(ret);
	}
}
