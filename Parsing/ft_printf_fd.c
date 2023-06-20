#include <unistd.h>
#include <stdarg.h>

int	printc(char c, int fd)
{
	write(fd, &c, 1);
	return (1);
}

int	prints(char *s, int fd)
{
	int	i;

	i = 0;
	if (!s)
		i += write(1, "(null)", 6);
	if (s)
	{
		while (s[i])
			printc(s[i++], fd);
	}
	return (i);
}

int	ft_specifier(va_list args, char specifier, int fd)
{
	int	printed;

	printed = 0;
	if (specifier == 'c')
		printed += printc(va_arg(args, int), fd);
	else if (specifier == 's')
		printed += prints(va_arg(args, char *), fd);
	else if (specifier == '%')
		printed += printc('%', fd);
	return (printed);
}

int	ft_printf(const char *first, int fd, ...)
{
	int		i;
	int		out;
	va_list	args;

	i = 0;
	out = 0;
	va_start(args, fd);
	while (first[i])
	{
		if (first[i] == '%')
		{
			out += ft_specifier(args, first[i + 1], fd);
			i++;
		}
		else
			out += write(fd, &first[i], 1);
		i++;
	}
	va_end(args);
	return (out);
}
