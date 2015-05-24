#include <stdio.h>
#include <stdlib.h>
#include "reordenamiento.h"
#include "../sparse/sp_packcol.h"

/** \file reordenamiento.c
 *	\brief Implementación de algoritmos de reordenamiento
 *
 */

/** \fn sp_packcol* permutar_packcol(unsigned int *p, sp_packcol *A)
 *	\brief Aplica una permutación de columnas sobre la matriz \c A
 *
 *	\param p Permutación. <TT>p[i]</TT> = La p[i]-ésima columna pasa a ser la columna i
 *	\param A Matriz a permutarle las columnas
 *	\return La misma matriz \c A pero con las matrices permutadas según \c p
 *
 *	\note
 *	La matriz \c A y el resultado no comparten memoria.
 */
sp_packcol* permutar_packcol(unsigned int *p, sp_packcol *A)
{
	unsigned int i, j, col;
	sp_packcol* B;

	B = (sp_packcol*)malloc(sizeof(sp_packcol));
	B->nrow = A->nrow;
	B->ncol = A->ncol;
	B->nnz = A->nnz;

	if (B->nnz == 0) {
		B->colp = NULL;
		B->rx = NULL;
		B->val = NULL;
	}
	else {
		B->colp = (unsigned int*)malloc(sizeof(unsigned int) * (B->ncol+1));
		B->rx = (unsigned int*)malloc(sizeof(unsigned int) * B->nnz);
		B->val = (double*)malloc(sizeof(double) * B->nnz);

		j = 0;
		for(col=0; col < B->ncol; ++col) {	/*< Para cada columna col en B */
			B->colp[col] = j;					/*< Los elementos de la columna col comenzarán en la entrada j */
			for (i=A->colp[p[col]]; i < A->colp[p[col]+1]; ++i) {	/*< Recorre la columna p[col]-ésima de A */
				B->rx[j] = A->rx[i];								/*< y copia el contenido */
				B->val[j] = A->val[i];
				++j;
			}
		}
		B->colp[col] = j;					/*< Elemento extra para indicar el fin de la ultima columna */
	}

	return B;
}

/** \fn unsigned int* reordenar_packcol(sp_packcol *A);
 *	\brief Reordena las columnas de A para minimizar el <em>fill-in</em> producido por una etapa posterior de factorización.
 *
 *	\param A Matriz a reordenar
 *
 *	\todo Implementar un algoritmo de reordenamiento
 *
 *	Actualmente esta función no implementa ningún algoritmo de reordenamiento. Simplemente retorna la permutación
 *	trivial para la cantidad de columnas de \c A.
 *
 *	Un algoritmo de reordenamiento básicamente calcula una permutación a ser aplicada en las
 *	filas/columnas de la matriz.
 *
 *	Mientras que no se cuente con una implementación de un algoritmo de reordenamiento,
 *	se puede calcular una permutación mediante código externo y utilizar la función permutar_packcol(),
 *	que recibe una permutación como parámetro y realiza la permutación sobre una matriz empaquetada por columna.
 *
 *	\note
 *	Para aquel que pretenda implementar un algoritmo de reordenamiento para BAL, una posibilidad es el
 *	algoritmo de Cuthill-McKee. Puede realizarse una implementación de cero, o tratar de reutilizar una
 *	existente, como la disponible en el siguiente sitio:
 *	http://www.math.temple.edu/~daffi/software/rcm/
 */
unsigned int* reordenar_packcol(sp_packcol *A)
{
	unsigned int *p;
	unsigned int i;

	p = (unsigned int*)malloc(sizeof(unsigned int) * A->ncol);

	for (i=0; i < A-> ncol; ++i)
		p[i] = i;

	return p;
}

