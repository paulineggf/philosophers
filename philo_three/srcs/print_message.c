/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_message.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pganglof <pganglof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/02 18:30:29 by pganglof          #+#    #+#             */
/*   Updated: 2020/07/02 18:41:47 by pganglof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

void			thread_print_two(t_philo *philo)
{
	int		time_len;

	time_len = ft_strlen(philo->time);
	write(1, philo->time, time_len);
	write(1, " ms: ", 5);
	write(1, philo->nb, philo->nb_len);
	if (philo->state == EAT)
	{
		write(1, " has taken 2 forks\n", 19);
		write(1, philo->time, time_len);
		write(1, " ms: ", 5);
		write(1, philo->nb, philo->nb_len);
		write(1, " is eating\n", 11);
	}
	else if (philo->state == SLEEP)
		write(1, " is sleeping\n", 13);
	else if (philo->state == THINK)
		write(1, " is thinking\n", 13);
	else
		write(1, " died\n", 6);
}

static void		*thread_print(void *arg)
{
	t_philo *philo;

	philo = (t_philo*)arg;
	sem_wait(philo->set->message);
	if (philo->set->died == 1)
		sem_post(philo->set->message);
	else
	{
		if (philo->state == DIED)
			philo->set->died = 1;
		thread_print_two(philo);
		sem_post(philo->set->message);
	}
	free(philo->time);
	return (NULL);
}

int				print_message(t_philo *philo, int str)
{
	philo->state = str;
    if (!(philo->time = ft_itoa(get_time() - philo->set->start_time)))
	{
		write(1, "\nMalloc error\n", 14);
		return (0);
	}
	if (pthread_create(&(philo->tid_message), NULL, &thread_print, philo) != 0)
	{
		write(1, "\nCan't create thread\n", 20);
		return (0);
	}
	pthread_join(philo->tid_message, NULL);
	if (philo->set->died == 1)
		return (0);
	return (1);
}
