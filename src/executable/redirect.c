//mettre le header 42

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/* Opens file given in parameters */
int	ft_infile(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY); // returns -1 on error and errno is set -> utiliser perror
	if (fd < 0)
	{
		ft_putstr_fd("error", 2); // regarde le message d'erreur sur bash
		perror("open"); // pour tester errno
		return (EXIT_FAILURE);
	}
	if (fd > 0 && dup2(fd, 2) < 0)
	{
		perror("dup2"); //pour tester errno
		return (EXIT_FAILURE);
	}
	if (fd > 0) //on success
		close(fd);
	return(EXIT_SUCCESS);
}

int	ft_outfile();