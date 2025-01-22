/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_module.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yustinov <yustinov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 14:44:02 by yustinov          #+#    #+#             */
/*   Updated: 2025/01/21 17:33:55 by yustinov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static pthread_mutex_t	*ft_init_forks(t_config *config)
{
	pthread_mutex_t	*forks;
	unsigned int	i;

	forks = malloc(sizeof(pthread_mutex_t) * config->np_philos);
	if (NULL == forks)
		return (ft_error_null(STR_ERR_MALLOC, NULL, 0));
	i = 0;
	while (i < config->np_philos)
	{
		if (pthread_mutex_init(&forks[i], 0) != 0)
			return (ft_error_null(STR_ERR_MUTEX, NULL, 0));
		i++;
	}
	return (forks);
}

static bool	ft_init_global_mutexes(t_config *config)
{
	config->fork_locks = ft_init_forks(config);
	if (NULL == config->fork_locks)
		return (false);
	if (pthread_mutex_init(&config->finish_lock, 0) != 0)
		return (ft_error_msg(STR_ERR_MUTEX, NULL, config));
	if (pthread_mutex_init(&config->printf_lock, 0) != 0)
		return (ft_error_msg(STR_ERR_MUTEX, NULL, config));
	return (true);
}

static void	ft_assign_forks(t_thread *philo)
{
	philo->fork[0] = philo->id;
	philo->fork[1] = (philo->id + 1) % philo->config->np_philos;
	if (philo->id % 2)
	{
		philo->fork[0] = (philo->id + 1) % philo->config->np_philos;
		philo->fork[1] = philo->id;
	}
}

static t_thread	**ft_init_philosophers(t_config *config)
{
	t_thread		**philos;
	unsigned int	i;

	philos = malloc(sizeof(t_thread) * config->np_philos);
	if (NULL == philos)
		return (ft_error_null(STR_ERR_MALLOC, NULL, 0));
	i = 0;
	while (i < config->np_philos)
	{
		philos[i] = malloc(sizeof(t_thread) * 1);
		if (NULL == philos[i])
			return (ft_error_null(STR_ERR_MALLOC, NULL, 0));
		if (pthread_mutex_init(&philos[i]->meal_time_lock, 0) != 0)
			return (ft_error_null(STR_ERR_MUTEX, NULL, 0));
		philos[i]->config = config;
		philos[i]->id = i;
		philos[i]->times_ate = 0;
		ft_assign_forks(philos[i]);
		i++;
	}
	return (philos);
}

t_config	*ft_init_config(int argc, char *argv[], int i)
{
	t_config	*config;

	config = (t_config *)malloc(sizeof(t_config) * 1);
	if (NULL == config)
		return (ft_error_null(STR_ERR_MALLOC, NULL, 0));
	config->np_philos = ft_atoi(argv[i++]);
	config->time_to_die = ft_atoi(argv[i++]);
	config->time_to_eat = ft_atoi(argv[i++]);
	config->time_to_sleep = ft_atoi(argv[i++]);
	config->must_eat_count = -1;
	if (argc - 1 == 5)
		config->must_eat_count = ft_atoi(argv[i]);
	config->philos = ft_init_philosophers(config);
	if (NULL == config->philos)
		return (NULL);
	if (!ft_init_global_mutexes(config))
		return (NULL);
	config->finish = false;
	return (config);
}
