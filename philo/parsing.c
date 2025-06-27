/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 12:56:31 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/27 18:52:25 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	ft_isspace(char c)
{
	return (c == 32 || (c >= 9 && c <= 13));
}

static bool	ft_isdigit(char c)
{
	return (c >= 48 && c <= 57);
}

static long	spec_atol(char *s)
{
	long	res;
	int	i;

	res = 0;
	i = 0;
	while (s[i] && ft_isspace(s[i]))
		i++;
	if (s[i] && (s[i] == '-' || s[i] == '+'))
	{
		if (s[i] == '-')
			handle_error(NEGATIVE, 2);
		i++;
	}
	if (s[i] && !ft_isdigit(s[i]))
		handle_error(NON_INT, 2);
	while (s[i] && ft_isdigit(s[i]))
	{
		res = res * 10 + s[i] - '0';
		if (res > INT_MAX)
			handle_error(NON_INT, 2);
		i++;
	}
	return (res);
}

void	add_and_check_arguments(char **argv, t_data *table)
{
	if (argv[5])
		table->n_meals = spec_atol(argv[5]);
	else
		table->n_meals = -1;
	if (table->n_meals == 0)
		handle_error(NO_MEALS, 2);
	table->n_philos = spec_atol(argv[1]);
	table->t_die = spec_atol(argv[2]) * 1e3;
	table->t_eat = spec_atol(argv[3]) * 1e3;
	table->t_sleep = spec_atol(argv[4]) * 1e3;
		table->n_meals = -1;
}
