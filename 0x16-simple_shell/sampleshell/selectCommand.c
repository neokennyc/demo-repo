/**
 * selectcommand - finds command
 * @stdinfo: the parameter struct
 * Return: -1 if command not found,
 * return 0 if builtin executed successfully,
 * else 1 if command found but not successful,
 * else 2 if command signals exit()
 */
int find_command(info_t *info)
{
	int i, ret = -1;
	command_table commandtbl[] = {
		{"exit", _myexit},
		{"env", _myenv},
		{"help", _myhelp},
		{"history", _myhistory},
		{"setenv", _mysetenv},
		{"unsetenv", _myunsetenv},
		{"cd", _mycd},
		{"alias", _myalias},
		{NULL, NULL}
	};

	for (i = 0; commandtbl[i].type; i++)
		if (_strcmp(info->argv[0], commandtbl[i].type) == 0)
		{
			/*info->line_count++;*/
			ret = commandtbl[i].func(info);
			break;
		}
	return (ret);
}

