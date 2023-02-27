/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: festeve- <festeve-@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 12:09:03 by festeve-          #+#    #+#             */
/*   Updated: 2023/02/27 08:17:41 by festeve-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdarg.h>
#include <unistd.h>
#include <limits.h>

typedef struct	s_sc
{
	int len;
	int width;
}				t_sc;

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putstr(char *s)
{
	int i;

	i = 0;
	while (s[i])
	{
		ft_putchar(s[i]);
		i++;
	}
}

int	ft_strlen(const char *s)
{
	int i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strchr(const char *s)
{
	while (*s)
	{
		if (*s == '%')
			return ((char *)s);
		s++;
	}
	if (!s)
		return ((char *)s);
	return (NULL);
}

void	ft_putnbr(int nb)
{
	if (nb == INT_MIN)
	{
		write(1, "-2147483648", 11);
		return ;
	}
	if (nb >= 0 && nb <= 9)
		ft_putchar(nb + 48);
	else if (nb < 0)
	{
		ft_putchar('-');
		ft_putnbr(-nb);
	}
	else
	{
		ft_putnbr(nb / 10);
		ft_putnbr(nb % 10);
	}
}

int	ft_intlen(int nb, char c)
{
	int i;
	int number;
	int neg;

	i = 0;
	if (!nb)
		return (1);
	if (nb < 0)
	{
		neg = 1;
		number = -nb;
	}
	else
	{
		neg = 0;
		number = nb;
	}
	if (c == 'd')
	{
		while (number)
		{
			number /= 10;
			i++;
		}
		return (i + neg);
	}
	if (c == 'x')
	{
		while (number)
		{
			number /= 16;
			i++;
		}
		return (i);
	}
	return (0);
}

void	ft_printhexa(unsigned int x)
{
	char *hexa = "0123456789abcdef";
	int res[100];
	int i;

	i = 0;
	while (x >= 16)
	{
		res[i] = hexa[x % 16];
		x = x / 16;
		i++;
	}
	res[i] = hexa[x];
	while (i >= 0)
	{
		ft_putchar(res[i]);
		i--;
	}
}

const char	*ft_search_arg(va_list arg, const char *fmt, t_sc *sc)
{
	if (*fmt == 'd')
	{
		int d;

		d = va_arg(arg, int);
		ft_putnbr(d);
		// ft_intlen?? 
		sc->len += ft_intlen(d, *fmt);
	}
	else if (*fmt == 's')
	{
		char *s;
	   
		s = va_arg(arg, char *);
		if (!s)
		{
			write (1, "(null)", 6);
			sc->len += 6;
		}
		else
		{
			ft_putstr(s);
			sc->len += ft_strlen(s);
		}
	}
	else if (*fmt == 'x')
	{
		unsigned int x;

		x = va_arg(arg, unsigned int);
		// ft_printhexa??
		ft_printhexa(x);
		sc->len += ft_intlen((int)x, *fmt);
	}
	else
		return (NULL);
	fmt++;
	return (fmt);
}

const char	*ft_read_text(t_sc *sc, const char *fmt)
{
	char *next;

	next = ft_strchr(fmt);
	if (next)
		sc->width = next - fmt;
	else
		sc->width = ft_strlen(fmt);
	write(1, fmt, sc->width);
	sc->len += sc->width;
	while (*fmt && *fmt != '%')
		++fmt;
	return(fmt);
}

int	ft_printf(const char *fmt, ...)
{
	va_list arg;
	va_start(arg, fmt);
	t_sc sc;
	sc.len = 0;
	sc.width = 0;

	while (*fmt)
	{
		if (*fmt == '%')
			fmt = ft_search_arg(arg, fmt + 1, &sc);
		else
			fmt = ft_read_text(&sc, fmt);
		if (!fmt)
		{
			write(1, "(null)", 6);
			va_end(arg);
			return (sc.len);
		}
	}
	va_end(arg);
	return(sc.len);
}

#include <stdio.h>

int	main()
{
	int res = ft_printf("Coucou %d\n", 55);
	int res2 = printf("Coucou %d\n", 55);

	printf("\nres = %d\nres2 = %d\n", res, res2);
}

