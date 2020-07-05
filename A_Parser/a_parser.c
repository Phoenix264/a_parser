/*
Автор: Алексеев А.Н.
Дата: 04.07.20

Описание: парсер разбора конструкций вида a{i;i;a{i;a{i;}}i;a{i;}a{}}.
Имеется два типа данных: примитивный и составной.
Составной тип - массив, начинается c "a{" заканчивается "}".
Примитивный тип состоит из двух символов: "i" и ";". Пример: a{i;i;a{}} - это массив из трех элементов: двух примитивных элементов и одного массива без элементов.

Входные данные:
a{i;i;a{i;a{i;}}i;a{i;}a{}}

Результат:
1 2 1 0 6
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "a_parser.h"
#include "a_parser.h"

/// Список элементов массивов одной глубины вложенности
typedef struct
{
	/// Количество составных элементов
	uint32_t count;
	/// Количество элементов в составных элементах
	uint32_t* elements;
} ArrayElementS;

/// Структура дерева информации об элементах
typedef struct
{
	/// Максимальная глубина вложенности элементов
	uint32_t depth;
	///  Данные об элементах массива одной глубины вложенности
	ArrayElementS arrayElements[MAX_DEPTH];
	///  Текущий разобранный эелемент
	CalcStates currentState;
	/// Текущая глубина вложенности разбираемого элемента
	uint32_t currentDepth;
} InfoTreeS;

/// Дерево информации об элементах
static InfoTreeS InfoTree;

/// Запуск разбора выражения
void search_start()
{
	InfoTree.depth = 0;
	InfoTree.currentState = START_STATE;
	InfoTree.currentDepth = 0;
}

/// Завершение разбора выражения
void search_end()
{
	for (uint32_t i = 0; i < InfoTree.depth; i++)
	{
		if (InfoTree.arrayElements[i].count > 0)
		{
			free(InfoTree.arrayElements[i].elements);
			InfoTree.arrayElements[i].elements = 0;
			InfoTree.arrayElements[i].count = 0;
		}
	}

	InfoTree.depth = 0;
	InfoTree.currentState = UNKNOWN_STATE;
	InfoTree.currentDepth = 0;
}

/// Обработка символа начала массива
static int start_array()
{
	if (InfoTree.depth == 0)
	{
		InfoTree.currentDepth = 1;
		InfoTree.depth = 1;
		InfoTree.arrayElements[0].elements = (uint32_t*)calloc(1, sizeof(uint32_t));
		InfoTree.arrayElements[0].count = 1;
	}
	else
	{
		if (InfoTree.currentDepth < InfoTree.depth)
		{
			InfoTree.arrayElements[InfoTree.currentDepth].count++;
			if (realloc(InfoTree.arrayElements[InfoTree.currentDepth].elements
				, InfoTree.arrayElements[InfoTree.currentDepth].count *
				sizeof(uint32_t)) == 0)
				return -1;
			InfoTree.arrayElements[InfoTree.currentDepth].elements[InfoTree.arrayElements[InfoTree.currentDepth].count - 1] = 0;
		}
		else
		{
			if (InfoTree.depth < MAX_DEPTH)
			{
				InfoTree.arrayElements[InfoTree.depth].count = 1;
				InfoTree.arrayElements[InfoTree.depth].elements = (uint32_t*)calloc(1, sizeof(uint32_t));
				InfoTree.depth++;
			}
			else
				return -1;

		}
		InfoTree.currentDepth++;
	}

	return 0;
}

/// Обработка символа конца массива
static void close_array()
{
	InfoTree.currentDepth--;
}

/// Добавление найденного элемента
static void add_element()
{
	InfoTree.arrayElements[InfoTree.currentDepth - 1].elements[InfoTree.arrayElements[InfoTree.currentDepth - 1].count - 1]++;
}

/// Печать резальтата разбора
void print_array()
{
	for (int i = InfoTree.depth - 1; i >= 0; i--)
	{
		for (uint32_t j = 0; j < InfoTree.arrayElements[i].count; j++)
		{
			fprintf(stdout, "%d\n", InfoTree.arrayElements[i].elements[j]);
		}
	}
}
/// Разбор следующего элемента
/// \param elem Следующий элемент для разбора
/// \return Результат разбора из списка SerachResult
int search_next(const char elem)
{
	switch (elem)
	{
	case 'a':
		if (InfoTree.currentState != I_STATE && InfoTree.currentState != A_STATE)
		{
			InfoTree.currentState = A_STATE;
			return SEARCH_NEXT;
		}
		else
		{
			return SEARCH_ERROR;
		}
		break;
	case 'i':
		if (InfoTree.currentState == P_STATE || InfoTree.currentState == L_STATE || InfoTree.currentState == R_STATE)
		{
			InfoTree.currentState = I_STATE;
			return SEARCH_NEXT;
		}
		else
		{
			return SEARCH_ERROR;
		}
		break;
	case '{':
		if (InfoTree.currentState == A_STATE)
		{
			if (start_array())
				return SEARCH_MEM_ERROR;
			InfoTree.currentState = L_STATE;
			return SEARCH_NEXT;
		}
		else
		{
			return SEARCH_ERROR;
		}
		break;
	case '}':
		if (InfoTree.currentState == L_STATE || InfoTree.currentState == P_STATE || InfoTree.currentState == R_STATE)
		{
			close_array();
			if (InfoTree.currentDepth != 0)
				add_element();

			InfoTree.currentState = R_STATE;
			if (InfoTree.currentDepth)
				return SEARCH_NEXT;
			else
				return SEARCH_END;
		}
		else
		{
			return SEARCH_ERROR;
		}
		break;
	case ';':
		if (InfoTree.currentState == I_STATE)
		{
			add_element();
			InfoTree.currentState = P_STATE;
			return SEARCH_NEXT;
		}
		else
		{
			return SEARCH_ERROR;
		}
		break;
	case '\n':
	case '\r':
		return SEARCH_NEXT;
		break;
	}

	return SEARCH_ERROR;
}
