/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_module.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yustinov <yustinov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:13:36 by yustinov          #+#    #+#             */
/*   Updated: 2025/01/22 15:48:46 by yustinov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	ft_msg(char *str, char *detail, int exit_no)
{
	if (!detail)
		printf(str, STR_PROG_NAME);
	else
		printf(str, STR_PROG_NAME, detail);
	return (exit_no);
}

int	ft_erro_fail(char *str, char *details, t_config *config)
{
	if (config != NULL)
		ft_free_config(config);
	return (ft_msg(str, details, 0));
}

void	*ft_err_null(char *str, char *details, t_config *config)
{
	if (config != NULL)
		ft_free_config(config);
	ft_msg(str, details, EXIT_FAILURE);
	return (NULL);
}

void	ft_child_exit(t_config *config, int exit_code)
{
	sem_post(config->this_philo->sem_meal);
	pthread_join(config->this_philo->observer, NULL);
	if (exit_code == CHILD_EXIT_ERR_SEM)
		ft_msg(STR_ERR_SEM, NULL, 0);
	if (exit_code == CHILD_EXIT_ERR_PTHREAD)
		ft_msg(STR_ERR_THREAD, NULL, 0);
	sem_close(config->this_philo->sem_forks);
	sem_close(config->this_philo->sem_philo_full);
	sem_close(config->this_philo->sem_philo_dead);
	sem_close(config->this_philo->sem_write);
	sem_close(config->this_philo->sem_meal);
	ft_free_config(config);
	exit(exit_code);
}
