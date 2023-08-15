/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 17:19:09 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/07/28 16:15:52 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

const char	*ft_search_arg_fd(va_list arg, const char *format, size_t *char_count, int fd)
{
	if (*format == 'd' || *format == 'i')
		ft_dputnbr_base(va_arg(arg, int), "0123456789", char_count, fd);
	if (*format == 'u')
		ft_dputnbr_base(va_arg(arg, unsigned int), "0123456789", char_count, fd);
	if (*format == 'x')
		ft_dputnbr_base(va_arg(arg, unsigned int),
			"0123456789abcdef", char_count, fd);
	if (*format == 'X')
		ft_dputnbr_base(va_arg(arg, unsigned int),
			"0123456789ABCDEF", char_count, fd);
	if (*format == 's')
		ft_putstr_dpf(va_arg(arg, const char *), char_count, fd);
	if (*format == 'c')
		ft_putchar_dpf(va_arg(arg, int), char_count, fd);
	if (*format == '%')
		ft_putchar_dpf('%', char_count, fd);
	if (*format == 'p')
		ft_putnbr_base_dp(va_arg(arg, size_t),
			"0123456789abcdef", char_count, fd);
	format++;
	return (format);
}

const char	*ft_puttext_fd(size_t *char_count, const char *format, int fd)
{
	const char	*next;
	size_t		string_length;

	next = ft_strchr(format, '%');
	if (next)
		string_length = next - format;
	else
		string_length = ft_strlen(format);
	write (fd, format, string_length);
	while (*format && *format != '%')
		++format;
	*char_count += string_length;
	return (format);
}

int	ft_dprintf(int fd, const char *format, ...)
{
	va_list	arg;
	size_t	char_count;

	char_count = 0;
	if (write(fd, 0, 0))
		return (-1);
	va_start(arg, format);
	while (*format)
	{
		if (*format == '%')
			format = ft_search_arg_fd(arg, format + 1, &char_count, fd);
		else
			format = ft_puttext_fd(&char_count, format, fd);
		if (!format)
		{
			write(fd, "(null)", 6);
			va_end(arg);
			return (6);
		}
	}
	va_end(arg);
	return ((int) char_count);
}
