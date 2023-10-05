//RAJOUTER HEADER

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

void    redirecters_error(char *line)
{
    int i;

    i = 0;
    if (ft_strlen(line) <= 2)
        printf("bash: syntax error near unexpected token `newline'\n");
    if (ft_strlen(line) > 2)
    {
        while (line[i] != ' ')
            i++;
        if (i > 1 && line[0] == '>')
            printf("bash: syntax error near unexpected token `>>'\n");
        if (i > 1 && line[0] == '<')
            printf("bash: syntax error near unexpected token `<<'\n");
        if (i == 1 && line[0] == '>')
            printf("bash: syntax error near unexpected token `>'\n");
        if (i == 1 && line[0] == '<')
            printf("bash: syntax error near unexpected token `<'\n");
    }
}
