/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_separate_work.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 12:01:34 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/21 13:16:28 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "preliminary.h"

int	nums[10] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };

static void	*routine(void *arg)
{
 	int	value;
	int	sum = 0;
	value = *(int *)arg;
	printf("The %d halfsum is comprised of ", value / 5 + 1);
	for (int i = 0; i < 5; i++)
	{
		sum += nums[value + i];
		printf("%d ", nums[value + i]);
	}
	printf(" and adds up to %d\n", sum);
	*(int *)arg = sum;
	return (arg);
}

void	threads_separate_work(void)
{
	pthread_t	th[2];
	int		total_sum = 0;
	for (size_t i = 0; i < 2; i++)
	{
		int	*a = malloc(4);
		*a = i * 5;
		if (pthread_create(&th[i], NULL, routine, a) != 0) 
			exit(EXIT_FAILURE);
	}
	for (size_t i = 0; i < 2; i++)
	{
		int	*r;
		if (pthread_join(th[i], (void **)&r) != 0)
			exit(EXIT_FAILURE);
		total_sum += *r;
		free(r);
	}
	printf("Total: %d\n", total_sum);
}
