input_file <ls >file -a | wc

cmd->content = ls
cmd-> type = 0
cmd->content->next = -a
cmd-> type = 1

file -->
cmd -> content = file
cmd-> type = 4




cmd_tab[0] = ls 0/
cmd_tab[1] = file
cmd_tab[2] = -a


cmd_tab[2] = wc
cmd_tab[3] = -l
cmd_tab[4]

cmd = cmd->next;
i++;

cmd[0]

cmd[0]->content -> char * content = ls;
cmd[0]->input -> char * input = intfile;
char *output = 
char *heredoc =



envoyer a l'executable les commandes une par une (la totalite jusqu'a la pipe)
while (cmd && cmd->type != PIPE)
{
	if (cmd->type != PIPE)
		//send to exec
	cmd = cmd->next;
}
