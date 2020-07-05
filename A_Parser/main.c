/*
Автор: Алексеев А.Н.
Дата: 04.07.20

Описание: проверка парсера разбора
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "a_parser.h"

int main(int argc, char* argv[])
{
	char buf;
	int result;

	search_start();
	while (1)
	{
		fread(&buf, 1, 1, stdin);
		if ((result = search_next(buf)) != SEARCH_NEXT)
		{
			if (result == SEARCH_END)
			{
				break;
			}
			else if (result == SEARCH_ERROR)
			{
				fprintf(stdout, "Syntax error");
			}
			else if (result == SEARCH_MEM_ERROR)
			{
				fprintf(stdout, "Memory error");
			}

			else
			{
				fprintf(stdout, "Unknown error");
			}
			search_end();
			return -1;
		}
	}

	print_array();
	search_end();

	return 0;
}
