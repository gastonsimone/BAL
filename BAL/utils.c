#include "utils.h"

/** \file utils.c
 *  \brief Implementación de utilidades generales
 */

/** \fn int binary_search(int *list, unsigned int size, int key)
 *  \brief Busca un elemento en el array mediante bipartición
 *
 *  \param list Arreglo de elementos
 *  \param size Tamaño de la lista
 *  \param key Elemento a buscar
 *  \return El índice en donde se ubica el elemento en el arreglo, -1 si no fue encontrado
 *
 *  Pos más información ver
 *  http://es.wikipedia.org/wiki/B%C3%BAsqueda_dicot%C3%B3mica
 */
int binary_search(int *list, unsigned int size, int key)
{
	int izq, der, medio;

	izq = 0;
	der = size - 1;

	while (izq <= der)
	{
		medio = (int)((izq + der) / 2);
		if (key == list[medio])
			return medio;
		else if (key > list[medio])
			izq = medio + 1;
		else
			der = medio - 1;
	}

	return -1;
}

/** \fn void insert_sorted(int *list, int n, int key)
 *  \brief Inserta un elemento en un arreglo ordenado
 *
 *  \param list Arreglo ordenado
 *  \param n Posición del último elemento del arreglo
 *  \param key elemento a insertar
 *
 *  Inserta un elemento en un array ordenado preservando el orden.
 *  Esta función corresponde al loop interno de un <em>insertion sort</em>.
 *  Por más información ver
 *  http://es.wikipedia.org/wiki/Ordenamiento_por_inserci%C3%B3n
 *
 *  \warning
 *  Esta función no solicita memoria. Se asume que la cantidad de memoria
 *  reservada es suficiente como para albergar al nuevo elemento.
 */
void insert_sorted(int *list, int n, int key)
{
	int pos = n;

	while (pos >= 0 && list[pos] > key) {
		list[pos+1] = list[pos];
		--pos;
	}
	list[pos+1] = key;
}

