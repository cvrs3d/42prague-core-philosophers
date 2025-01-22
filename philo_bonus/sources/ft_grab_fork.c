/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_grab_fork.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yustinov <yustinov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 14:37:53 by yustinov          #+#    #+#             */
/*   Updated: 2025/01/22 14:42:25 by yustinov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	ft_grab_fork(t_philo *philo)
{
	sem_wait(philo->sem_forks);
	sem_wait(philo->sem_meal);
	if (philo->forks_acc <= 0)
		ft_write_status(philo, false, GOT_FORK_1);
	if (philo->forks_acc == 1)
		ft_write_status(philo, false, GOT_FORK_2);
	philo->forks_acc += 1;
	sem_post(philo->sem_meal);
}
