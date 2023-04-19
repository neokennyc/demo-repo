int (*select_output(char ch))(va_list, flags_t *)
{
	OUTS output_funcs[] = {
		{'i', output_ints},
		{'s', output_alpha},
		{'c', outtput_char},
		{'d', output_ints},
		{'u', output_unsignedint},
		{'x', print_hex},
		{'X', print_hex_big},
		{'b', print_binary},
		{'o', print_octal},
		{'R', print_rot13},
		{'r', print_rev},
		{'S', print_bigS},
		{'p', print_address},
		{'%', print_percent}
		};
	int keys = 14;

	int j;

	for (j= 0; j < keys; j++)
		if (output_funcs[i].spec == ch)
			return (output_funcs[j].selectprint);

	return (NULL);
