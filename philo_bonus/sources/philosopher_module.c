/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_module.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yustinov <yustinov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 13:59:49 by yustinov          #+#    #+#             */
/*   Updated: 2025/01/22 15:51:32 by yustinov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	eat_sleep_routine(t_philo *philo)
{
	ft_grab_fork(philo);
	ft_grab_fork(philo);
	ft_write_status(philo, false, EATING);
	sem_wait(philo->sem_meal);
	philo->last_meal = ft_get_time();
	sem_post(philo->sem_meal);
	ft_philo_sleep(philo->config->time_to_eat);
	ft_write_status(philo, false, SLEEPING);
	sem_post(philo->sem_forks);
	sem_post(philo->sem_forks);
	sem_wait(philo->sem_meal);
	philo->forks_acc -= 2;
	philo->times_ate += 1;
	sem_post(philo->sem_meal);
	ft_philo_sleep(philo->config->time_to_sleep);
}

static void	think_routine(t_philo *philo, bool silent)
{
	time_t	time_to_think;

	sem_wait(philo->sem_meal);
	time_to_think = (philo->config->time_to_die
			- (ft_get_time() - philo->last_meal)
			- philo->config->time_to_eat) / 2;
	sem_post(philo->sem_meal);
	if (time_to_think < 0)
		time_to_think = 0;
	if (time_to_think == 0 && silent == true)
		time_to_think = 1;
	if (time_to_think > 600)
		time_to_think = 200;
	if (silent == false)
		ft_write_status(philo, false, THINKING);
	ft_philo_sleep(time_to_think);
}

static void	lone_philo_routine(t_philo *philo)
{
	philo->sem_philo_full = sem_open(SEM_NAME_FULL, O_CREAT,
			S_IRUSR | S_IWUSR, philo->config->n_philos);
	if (philo->sem_philo_full == SEM_FAILED)
		exit(CHILD_EXIT_ERR_SEM);
	sem_wait(philo->sem_philo_full);
	ft_sim_delay(philo->config->start_time);
	if (philo->config->must_eat_count == 0)
	{
		sem_post(philo->sem_philo_full);
		exit(CHILD_EXIT_PHILO_FULL);
	}
	ft_print_status(philo, "has taken a fork.");
	ft_philo_sleep(philo->config->time_to_die);
	ft_print_status(philo, "died.");
	ft_free_config(philo->config);
	exit(CHILD_EXIT_PHILO_DEAD);
}

static void	philosoper_routine(t_philo *philo)
{
	if (philo->id % 2)
		think_routine(philo, true);
	while (1)
	{
		eat_sleep_routine(philo);
		think_routine(philo, false);
	}
}

void	ft_philosopher(t_config *config)
{
	t_philo	*philo;

	philo = config->this_philo;
	if (philo->config->n_philos == 1)
		lone_philo_routine(philo);
	ft_init_philo_ipc(config, philo);
	if (philo->config->must_eat_count == 0)
	{
		sem_post(philo->sem_philo_full);
		ft_child_exit(config, CHILD_EXIT_PHILO_FULL);
	}
	if (philo->config->time_to_die == 0)
	{
		sem_post(philo->sem_philo_dead);
		ft_child_exit(config, CHILD_EXIT_PHILO_DEAD);
	}
	sem_wait(philo->sem_meal);
	philo->last_meal = philo->config->start_time;
	sem_post(philo->sem_meal);
	ft_sim_delay(philo->config->start_time);
	philosoper_routine(philo);
}
