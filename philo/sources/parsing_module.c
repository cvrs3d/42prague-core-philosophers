/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_module.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yustinov <yustinov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 16:18:52 by yustinov          #+#    #+#             */
/*   Updated: 2025/01/21 17:34:43 by yustinov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	ft_is_digit(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
		i++;
	}
	return (true);
}

int	ft_atoi(char *str)
{
	unsigned long long int	nb;
	int						i;

	i = 0;
	nb = 0;
	while (str[i] && (str[i] >= '0' && str[i] <= '9'))
	{
		nb = nb * 10 + (str[i] - '0');
		i++;
	}
	if (nb > INT_MAX)
		return (-1);
	return ((int)nb);
}

bool	ft_is_valid_input(int argc, char *argv[])
{
	int	i;
	int	nb;

	i = 1;
	while (i < argc)
	{
		if (!ft_is_digit(argv[i]))
			return (ft_msg(STR_ERR_INPUT_DIGIT, argv[i], false));
		nb = ft_atoi(argv[i]);
		if (i == 1 && (nb <= 0 || nb > MAX_THREADS))
			return (ft_msg(STR_ERR_INPUT_POFLOW, STR_MAX_THREADS, false));
		if (i != 1 && nb == -1)
			return (ft_msg(STR_ERR_INPUT_DIGIT, argv[i], false));
		i++;
	}
	return (true);
}
