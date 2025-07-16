/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_id.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 12:57:32 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/21 13:00:29 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "preliminary.h"

static void	*print_id(void *arg)
{
	printf("Thread ID %lu\n", *(pthread_t *)arg);
	return (NULL);
}

void	threads_id(void)
{
	pthread_t	t1, t2;
	if (pthread_create(&t1, NULL, print_id, &t1) != 0)
		exit(EXIT_FAILURE);
	if (pthread_create(&t2, NULL, print_id, &t2) != 0)
		exit(EXIT_FAILURE);
	if (pthread_join(t1, NULL) != 0)
		exit(EXIT_FAILURE);
	if (pthread_join(t2, NULL) != 0)
		exit(EXIT_FAILURE);
}
