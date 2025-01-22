/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_module.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yustinov <yustinov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:49:14 by yustinov          #+#    #+#             */
/*   Updated: 2025/01/22 16:47:58 by yustinov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	ft_print_status(t_philo *philo, char *str)
{
	printf("%ld Philosopher%d %s\n", ft_get_time() - philo->config->start_time,
		philo->id + 1, str);
}

void	ft_write_status(t_philo *philo, bool reaper_report, t_status status)
{
	sem_wait(philo->sem_write);
	if (status == DIED)
		ft_print_status(philo, "died");
	else if (status == EATING)
		ft_print_status(philo, "is eating");
	else if (status == SLEEPING)
		ft_print_status(philo, "is sleeping");
	else if (status == THINKING)
		ft_print_status(philo, "is thinking");
	else if (status == GOT_FORK_1 || status == GOT_FORK_2)
		ft_print_status(philo, "has taken a fork");
	if (!reaper_report)
		sem_post(philo->sem_write);
}

void	ft_write_outcome(t_config *config)
{
	sem_post(config->sem_write);
	sem_wait(config->sem_write);
	printf("%d/%d philosophers had at least %d meals.\n",
		config->philo_full_count, config->n_philos, config->must_eat_count);
	sem_post(config->sem_write);
	return ;
}
