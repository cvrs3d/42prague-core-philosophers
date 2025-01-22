/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_module.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yustinov <yustinov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 15:52:01 by yustinov          #+#    #+#             */
/*   Updated: 2025/01/22 17:01:27 by yustinov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	ft_print_status_debug(t_thread *philo, char *color,
								char *str, t_status status)
{
	if (status == GOT_FORK_1)
		printf("[%10ld]\t%s%03d\t%s\e[0m: fork [%d]\n",
			ft_get_time() - philo->config->init_time,
			color, philo->id + 1, str, philo->fork[0]);
	else if (status == GOT_FORK_2)
		printf("[%10ld]\t%s%03d\t%s\e[0m: fork [%d]\n",
			ft_get_time() - philo->config->init_time,
			color, philo->id + 1, str, philo->fork[1]);
	else
		printf("[%10ld]\t%s%03d\t%s\e[0m\n",
			ft_get_time() - philo->config->init_time,
			color, philo->id + 1, str);
}

static void	ft_write_status_debug(t_thread *philo, t_status status)
{
	if (status == DIED)
		ft_print_status_debug(philo, RED, "died", status);
	else if (status == EATING)
		ft_print_status_debug(philo, GREEN, "is eating", status);
	else if (status == SLEEPING)
		ft_print_status_debug(philo, CYAN, "is sleeping", status);
	else if (status == THINKING)
		ft_print_status_debug(philo, CYAN, "is thinking", status);
	else if (status == GOT_FORK_1)
		ft_print_status_debug(philo, PURPLE, "has taken a fork", status);
	else if (status == GOT_FORK_2)
		ft_print_status_debug(philo, PURPLE, "has taken a fork", status);
}

static void	ft_print_status(t_thread *philo, char *str)
{
	printf("%ldms Philosopher%d %s\n", ft_get_time()
		- philo->config->init_time,
		philo->id + 1, str);
}

void	ft_write_status(t_thread *philo, bool report, t_status status)
{
	pthread_mutex_lock(&philo->config->printf_lock);
	if (ft_has_sim_stopped(philo->config) == true && report == false)
	{
		pthread_mutex_unlock(&philo->config->printf_lock);
		return ;
	}
	if (DEBUG_FORAMTING == true)
	{
		ft_write_status_debug(philo, status);
		pthread_mutex_unlock(&philo->config->printf_lock);
		return ;
	}
	if (DIED == status)
		ft_print_status(philo, "died");
	else if (status == EATING)
		ft_print_status(philo, "is eating");
	else if (status == SLEEPING)
		ft_print_status(philo, "is sleeping");
	else if (status == THINKING)
		ft_print_status(philo, "is thinking");
	else if (status == GOT_FORK_1 || status == GOT_FORK_2)
		ft_print_status(philo, "has taken a fork");
	pthread_mutex_unlock(&philo->config->printf_lock);
}

void	ft_write_outcome(t_config *config)
{
	unsigned int	i;
	unsigned int	full_count;

	full_count = 0;
	i = 0;
	while (i < config->np_philos)
	{
		if (config->philos[i]->times_ate
			>= (unsigned int)config->must_eat_count)
			full_count++;
		i++;
	}
	pthread_mutex_lock(&config->printf_lock);
	printf("%d%d philosophers had at least %d meals.\n",
		full_count, config->np_philos, config->must_eat_count);
	pthread_mutex_unlock(&config->printf_lock);
	return ;
}
