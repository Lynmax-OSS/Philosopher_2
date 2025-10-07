/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keteo <keteo@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 13:17:51 by keteo             #+#    #+#             */
/*   Updated: 2025/09/02 13:17:51 by keteo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>

typedef struct s_rules	t_rules;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long long		last_meal;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	meal_lock;
	t_rules			*rules;
}	t_philo;

typedef struct s_rules
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat;
	long long		start_time;
	int				someone_died;
	int				all_satiated;
	int				start_sim;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	state_lock;
	t_philo			*philos;
	pthread_t		monitor_thread;
}	t_rules;

int			parse_and_init(int ac, char **av, t_rules *r);
void		link_philos_and_forks(t_rules *r);
void		*philo_routine(void *arg);
void		*monitor_routine(void *arg);
long long	now_ms(void);
long long	elapsed_ms(long long since);
void		ms_sleep(t_rules *r, int ms);
int			safe_print(t_rules *r, int id, const char *msg);
int			str_to_int(const char *s, int *out);
void		destroy_and_free(t_rules *r);
void		waiting(t_philo *p);

#endif
