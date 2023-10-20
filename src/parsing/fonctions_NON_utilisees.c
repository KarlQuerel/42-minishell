/*Skips quotes and spaces in the command line.*/
int skip(char *line, int i, int option)
{
    if (option == SPACES)
    {
        while(line[i] == ' ' && line[i])
            i++;
    }
    if (option == QUOTES)
    {
        if (line[i] == '\'' || line[i] == '\"')
            i++;
    }
    return (i);
}

void	commands(t_element *current_cmd, t_env *env_list, char *home_path)
{
	// a la base j avais fait " cd " etc donc avec espace apres le 
	// nom du builtin pour eviter les erreur du type "holacdhey" et 
	// que ca compte comme cd mais pb : cd tout seul sans espace avant
	// car premiere cmd de la ligne et sans espace apres car c est a 
	// moi de remplacer par le home_path alors ca ne marche pas. Même
	// pb pour le $ dans la fonction line_errors_and_fix car on peut
	// avoir $ suivi de '?'.
	if (is_this_command(line, "history") == true)
		history(HISTORY);
	else if (is_this_command(line, "pwd") == true)
		pwd(PRINT);
	else if (is_cmd_in_line(line, "cd") == true)
		cd(line, home_path, env_list);
	else if (is_cmd_in_line(line, "echo") == true)
        echo(line);
}