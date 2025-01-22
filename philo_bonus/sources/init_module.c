/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_module.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yustinov <yustinov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 13:30:44 by yustinov          #+#    #+#             */
/*   Updated: 2025/01/22 16:09:18 by yustinov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static char	*set_local_sem_name(const char *str, unsigned int id)
{
	unsigned int	i;
	unsigned int	digit_count;
	char			*sem_name;
	char			*tmp;

	digit_count = 0;
	i = id;
	while (i)
	{
		digit_count++;
		i /= 10;
	}
	i = ft_strlen(str) + digit_count;
	sem_name = malloc(sizeof * sem_name * (i + 1));
	if (sem_name == NULL)
		return (NULL);
	sem_name[0] = '\0';
	sem_name = ft_strcat(sem_name, str);
	tmp = ft_utoa(id, digit_count);
	sem_name = ft_strcat(sem_name, tmp);
	free(tmp);
	return (sem_name);
}

static bool	set_philo_sem_names(t_philo *philo)
{
	philo->sem_meal_name = set_local_sem_name(SEM_NAME_MEAL, philo->id + 1);
	if (philo->sem_meal_name == NULL)
		return (false);
	return (true);
}

static t_philo	**init_philosophers(t_config *config)
{
	t_philo			**philos;
	unsigned int	i;

	philos = malloc(sizeof(t_philo) * (config->n_philos + 1));
	if (NULL == philos)
		return (ft_err_null(STR_ERR_MALLOC, NULL, 0));
	i = 0;
	while (i < config->n_philos)
	{
		philos[i] = malloc(sizeof(t_philo) * 1);
		if (!philos[i])
			return (ft_err_null(STR_ERR_MALLOC, NULL, 0));
		philos[i]->config = config;
		philos[i]->id = i;
		if (!set_philo_sem_names(philos[i]))
			return (ft_err_null(STR_ERR_MALLOC, NULL, config));
		philos[i]->times_ate = 0;
		philos[i]->forks_acc = 0;
		philos[i]->ate_enough = false;
		i++;
	}
	return (philos);
}

static bool	init_global_semaphores(t_config *config)
{
	ft_unlink_g_sems();
	config->sem_forks = sem_open(SEM_NAME_FORKS, O_CREAT,
			S_IRUSR | S_IWUSR, config->n_philos);
	if (config->sem_forks == SEM_FAILED)
		return (ft_sem_err_cleanup(config));
	config->sem_write = sem_open(SEM_NAME_WRITE, O_CREAT,
			S_IRUSR | S_IWUSR, 1);
	if (config->sem_write == SEM_FAILED)
		return (ft_sem_err_cleanup(config));
	config->sem_philo_full = sem_open(SEM_NAME_FULL, O_CREAT,
			S_IRUSR | S_IWUSR, config->n_philos);
	if (config->sem_philo_full == SEM_FAILED)
		return (ft_sem_err_cleanup(config));
	config->sem_philo_dead = sem_open(SEM_NAME_DEAD, O_CREAT,
			S_IRUSR | S_IWUSR, config->n_philos);
	if (config->sem_philo_dead == SEM_FAILED)
		return (ft_sem_err_cleanup(config));
	config->sem_stop = sem_open(SEM_NAME_STOP, O_CREAT,
			S_IRUSR | S_IWUSR, 1);
	if (config->sem_stop == SEM_FAILED)
		return (ft_sem_err_cleanup(config));
	return (true);
}

t_config	*ft_init_config(int argc, char *argv[], int i)
{
	t_config	*config;

	config = malloc(sizeof(t_config) * 1);
	if (NULL == config)
		return (ft_err_null(STR_ERR_MALLOC, NULL, 0));
	config->n_philos = ft_atoi(argv[i++]);
	config->time_to_die = ft_atoi(argv[i++]);
	config->time_to_eat = ft_atoi(argv[i++]);
	config->time_to_sleep = ft_atoi(argv[i++]);
	config->must_eat_count = -1;
	config->philo_full_count = 0;
	if (argc - 1 == 5)
		config->must_eat_count = ft_atoi(argv[i]);
	if (!init_global_semaphores(config))
		return (NULL);
	config->philos = init_philosophers(config);
	if (!config->philos)
		return (NULL);
	config->pids = malloc(sizeof * config->pids * config->n_philos);
	if (!config->pids)
		return (ft_err_null(STR_ERR_MALLOC, NULL, 0));
	return (config);
}
