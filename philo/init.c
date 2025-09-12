/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keteo <keteo@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 13:21:17 by keteo             #+#    #+#             */
/*   Updated: 2025/09/02 13:21:17 by keteo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_mutexes(t_rules *r)
{
	int	i;

	r->forks = malloc(sizeof(pthread_mutex_t) * r->num_philos);
	if (!r->forks)
		return (1);
	i = 0;
	while (i < r->num_philos)
	{
		if (pthread_mutex_init(&r->forks[i], NULL))
			return (1);
		i++;
	}
	if (pthread_mutex_init(&r->print_lock, NULL))
		return (1);
	if (pthread_mutex_init(&r->state_lock, NULL))
		return (1);
	return (0);
}

static int	init_philos(t_rules *r)
{
	int	i;

	r->philos = malloc(sizeof(t_philo) * r->num_philos);
	if (!r->philos)
		return (1);
	i = 0;
	while (i < r->num_philos)
	{
		r->philos[i].id = i + 1;
		r->philos[i].meals_eaten = 0;
		r->philos[i].last_meal = 0;
		r->philos[i].rules = r;
		i++;
	}
	link_philos_and_forks(r);
	return (0);
}

void	link_philos_and_forks(t_rules *r)
{
	int	i;

	i = 0;
	while (i < r->num_philos)
	{
		r->philos[i].left_fork = &r->forks[i];
		r->philos[i].right_fork = &r->forks[(i + 1) % r->num_philos];
		i++;
	}
}

static int	check_arg(char **av, t_rules *r)
{
	if (str_to_int(av[1], &r->num_philos) || r->num_philos < 1)
	{
		printf("num_philos is negative\n");
		return (1);
	}
	if (str_to_int(av[2], &r->time_to_die) || r->time_to_die < 0)
	{
		printf("time_to_die is negative\n");
		return (1);
	}
	if (str_to_int(av[3], &r->time_to_eat) || r->time_to_eat < 0)
	{
		printf("time_to_eat is negative\n");
		return (1);
	}
	if (str_to_int(av[4], &r->time_to_sleep) || r->time_to_sleep < 0)
	{
		printf("time_to_sleep is negative\n");
		return (1);
	}
	return (0);
}

int	parse_and_init(int ac, char **av, t_rules *r)
{
	if (ac != 5 && ac != 6)
	{
		write(2, "Usage: ./philo n t_die t_eat t_sleep [must_eat]\n", 48);
		return (1);
	}
	if (check_arg(av, r))
		return (1);
	r->must_eat = -1;
	if (ac == 6 && (str_to_int(av[5], &r->must_eat) || r->must_eat <= 0))
	{
		printf("must_eat is negative\n");
		return (1);
	}
	r->someone_died = 0;
	r->all_satiated = 0;
	if (init_mutexes(r) || init_philos(r))
		return (1);
	return (0);
}
