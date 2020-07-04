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

#pragma once

/// Максимальная глубина вложенности составных элементов
#define MAX_DEPTH 50

///  Разбираемые элементы
typedef enum
{
	/// Начало разбора, элемент не выбран
	START_STATE,
	/// Элемент 'a'
	A_STATE,
	/// Элемент '{'
	L_STATE,
	/// Элемент '}'
	R_STATE,
	/// Элемент ';'
	P_STATE,
	/// Элемент 'i'
	I_STATE,
	/// Неизвестный элемент
	UNKNOWN_STATE
} CalcStates;

///  Результаты парсинга
typedef enum
{
	/// Ошибка памяти
	SEARCH_MEM_ERROR = -2,
	/// Ошибка синтаксиса
	SEARCH_ERROR = -1,
	/// Ожидание разбора следующего элемента
	SEARCH_NEXT = 0,
	///  Завершение разбора
	SEARCH_END = 1,
} SerachResult;

/// Запуск разбора выражения
void search_start();

/// Разбор следующего элемента
/// \param elem Следующий элемент для разбора
/// \return Результат разбора из списка SerachResult
int search_next(const char elem);

/// Печать резальтата разбора
void print_array();

/// Завершение разбора выражения
void search_end();
