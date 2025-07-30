#include "minishell.h"
#include "libft.h"

int		check_infile_permission(t_parser *parser, char *infile)
{
	if (access(infile, F_OK) == -1)
		return (set_error(parser));
	if (access(infile, R_OK) == -1)
		return (set_error(parser));
	return (0);
}

int		check_outfile_permission(t_parser *parser, char *outfile)
{
	if (access(outfile, F_OK) == 0 && access(outfile, W_OK) == -1)
		return (set_error(parser));
	return (0);
}

