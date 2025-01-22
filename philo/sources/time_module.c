/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_module.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yustinov <yustinov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 16:09:30 by yustinov          #+#    #+#             */
/*   Updated: 2025/01/21 16:17:14 by yustinov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

time_t	ft_get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_thread_sleep(t_config *config, time_t sleep_time)
{
	time_t	wake_up;

	wake_up = ft_get_time() + sleep_time;
	while (ft_get_time() < wake_up)
	{
		if (ft_has_sim_stopped(config))
			break ;
		usleep(100);
	}
}

void	ft_sim_start_delay(time_t	start_time)
{
	while (ft_get_time() < start_time)
		continue ;
}
