#include "libft.h"

int	ft_snprintf(char *str, size_t size, const char *format, ...)
{
	va_list	args;
	int		ret;

	va_start(args, format);
	ret = vsnprintf(str, size, format, args);
	va_end(args);
	return (ret);
}
