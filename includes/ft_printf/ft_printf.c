/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 17:19:09 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/03/22 11:26:36 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

const char	*ft_search_arg(va_list arg, const char *format, size_t *char_count)
{
	if (*format == 'd' || *format == 'i')
		ft_putnbr_base(va_arg(arg, int), "0123456789", char_count);
	if (*format == 'u')
		ft_putnbr_base(va_arg(arg, unsigned int), "0123456789", char_count);
	if (*format == 'x')
		ft_putnbr_base(va_arg(arg, unsigned int),
			"0123456789abcdef", char_count);
	if (*format == 'X')
		ft_putnbr_base(va_arg(arg, unsigned int),
			"0123456789ABCDEF", char_count);
	if (*format == 's')
		ft_putstr_pf(va_arg(arg, const char *), char_count);
	if (*format == 'c')
		ft_putchar_pf(va_arg(arg, int), char_count);
	if (*format == '%')
		ft_putchar_pf('%', char_count);
	if (*format == 'p')
		ft_putnbr_base_p(va_arg(arg, size_t),
			"0123456789abcdef", char_count);
	format++;
	return (format);
}

const char	*ft_puttext(size_t *char_count, const char *format)
{
	const char	*next;
	size_t		string_length;

	next = ft_strchr(format, '%');
	if (next)
	{
		string_length = next - format;
	}
	else
		string_length = ft_strlen(format);
	write (1, format, string_length);
	while (*format && *format != '%')
		++format;
	*char_count += string_length;
	return (format);
}

int	ft_printf(const char *format, ...)
{
	va_list	arg;
	size_t	char_count;

	char_count = 0;
	if (write(1, 0, 0))
		return (-1);
	va_start(arg, format);
	while (*format)
	{
		if (*format == '%')
			format = ft_search_arg(arg, format + 1, &char_count);
		else
			format = ft_puttext(&char_count, format);
		if (!format)
		{
			write(1, "(null)", 6);
			va_end(arg);
			return (6);
		}
	}
	va_end(arg);
	return ((int) char_count);
}
