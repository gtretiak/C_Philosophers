/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 12:56:31 by gtretiak          #+#    #+#             */
/*   Updated: 2025/07/02 18:52:15 by gtretiak         ###   ########.fr       */
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
	int		i;

	res = 0;
	i = 0;
	while (s[i] && ft_isspace(s[i]))
		i++;
	if (s[i] && (s[i] == '-' || s[i] == '+'))
	{
		if (s[i] == '-')
			return (-2);
		i++;
	}
	if (s[i] && !ft_isdigit(s[i]))
		return (-2);
	while (s[i] && ft_isdigit(s[i]))
	{
		res = res * 10 + (s[i] - '0');
		if (res > INT_MAX)
			return (-2);
		i++;
	}
	return (res);
}

int	add_and_check_arguments(char **argv, t_data *cafe)
{
	cafe->table = malloc(sizeof(t_common_data));
	if (cafe->table == NULL)
		return (handle_error(12, 0, MALLOC, cafe));
	if (argv[5])
		cafe->table->n_meals = spec_atol(argv[5]);
	else
		cafe->table->n_meals = -1;
	if (cafe->table->n_meals == 0)
		return (handle_error(22, 1, NO_MEALS, cafe));
	else if (cafe->table->n_meals == -2)
		return (handle_error(22, 1, INVALID, cafe));
	cafe->table->n_philos = spec_atol(argv[1]);
	if (cafe->table->n_philos == 0)
		return (handle_error(22, 1, NO_PHILOS, cafe));
	else if (cafe->table->n_philos < 0)
		return (handle_error(22, 1, INVALID, cafe));
	cafe->table->t_die = spec_atol(argv[2]);
	cafe->table->t_eat = spec_atol(argv[3]);
	cafe->table->t_sleep = spec_atol(argv[4]);
	if (cafe->table->t_die < 0
		|| cafe->table->t_eat < 0
		|| cafe->table->t_sleep < 0)
		return (handle_error(22, 1, INVALID, cafe));
	else if (cafe->table->t_die < 60
		|| cafe->table->t_eat < 60
		|| cafe->table->t_sleep < 60)
		return (handle_error(22, 1, NO_TIME, cafe));
	return (0);
}
