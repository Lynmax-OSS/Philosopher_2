/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keteo <keteo@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 13:22:01 by keteo             #+#    #+#             */
/*   Updated: 2025/09/02 13:22:01 by keteo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	now_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long long)tv.tv_sec * 1000 + (long long)tv.tv_usec / 1000);
}

long long	elapsed_ms(long long since)
{
	return (now_ms() - since);
}

void	ms_sleep(t_rules *r, int ms)
{
	long long	start;

	start = now_ms();
	while (elapsed_ms(start) < ms)
	{
		pthread_mutex_lock(&r->state_lock);
		if (r->someone_died || r->all_satiated)
		{
			pthread_mutex_unlock(&r->state_lock);
			break ;
		}
		pthread_mutex_unlock(&r->state_lock);
		usleep(100);
	}
}

int	safe_print(t_rules *r, int id, const char *msg)
{
	int	stop;

	pthread_mutex_lock(&r->state_lock);
	pthread_mutex_lock(&r->print_lock);
	stop = r->someone_died && (msg[0] != 'd');
	if (!stop && !r->all_satiated)
		printf("%lld %d %s\n", elapsed_ms(r->start_time), id, msg);
	pthread_mutex_unlock(&r->state_lock);
	pthread_mutex_unlock(&r->print_lock);
	return (0);
}

int	str_to_int(const char *s, int *out)
{
	long long	val;
	int			i;

	val = 0;
	i = 0;
	if (!s || !s[0])
		return (1);
	while (s[i] == ' ' || (s[i] >= 9 && s[i] <= 13))
		i++;
	if (s[i] == '+')
		i++;
	if (s[i] < '0' || s[i] > '9')
		return (1);
	while (s[i] >= '0' && s[i] <= '9')
	{
		val = val * 10 + (s[i] - '0');
		if (val > 2147483647)
			return (1);
		i++;
	}
	if (s[i] != '\0')
		return (1);
	*out = (int)val;
	return (0);
}
