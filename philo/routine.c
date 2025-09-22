/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keteo <keteo@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 13:21:36 by keteo             #+#    #+#             */
/*   Updated: 2025/09/02 13:21:36 by keteo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	take_forks(t_philo *p)
{
	if (p->id % 2 == 0)
	{
		pthread_mutex_lock(p->right_fork);
		safe_print(p->rules, p->id, "has taken a fork");
		pthread_mutex_lock(p->left_fork);
		safe_print(p->rules, p->id, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(p->left_fork);
		safe_print(p->rules, p->id, "has taken a fork");
		pthread_mutex_lock(p->right_fork);
		safe_print(p->rules, p->id, "has taken a fork");
	}
}

static void	put_forks(t_philo *p)
{
	pthread_mutex_unlock(p->left_fork);
	pthread_mutex_unlock(p->right_fork);
}

static int	is_over(t_rules *r)
{
	int	over;

	pthread_mutex_lock(&r->state_lock);
	over = r->someone_died || r->all_satiated;
	pthread_mutex_unlock(&r->state_lock);
	return (over);
}

static void	eat(t_philo *p)
{
	pthread_mutex_lock(&p->rules->state_lock);
	p->last_meal = now_ms();
	pthread_mutex_unlock(&p->rules->state_lock);
	safe_print(p->rules, p->id, "is eating");
	ms_sleep(p->rules, p->rules->time_to_eat);
	pthread_mutex_lock(&p->rules->state_lock);
	p->meals_eaten++;
	pthread_mutex_unlock(&p->rules->state_lock);
}

void	*philo_routine(void *arg)
{
	t_philo	*p;

	p = (t_philo *)arg;
	waiting(p);
	if (p->rules->num_philos == 1)
	{
		safe_print(p->rules, p->id, "has taken a fork");
		ms_sleep(p->rules, p->rules->time_to_die + 1);
		return (NULL);
	}
	if (p->id % 2 == 0)
		usleep(100);
	while (!is_over(p->rules))
	{
		take_forks(p);
		eat(p);
		put_forks(p);
		safe_print(p->rules, p->id, "is sleeping");
		ms_sleep(p->rules, p->rules->time_to_sleep);
		safe_print(p->rules, p->id, "is thinking");
	}
	return (NULL);
}
