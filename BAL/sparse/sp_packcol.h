#ifndef __SP_PACKCOL_H__
#define __SP_PACKCOL_H__

#include <stdio.h>
#include "sp_coord.h"

/** \file sp_packcol.h
 *	\brief Archivo de cabecera para matriz dispersa, formato empaquetado por columna.
 *
 *	Este archivo contiene la definición de la estructura de datos
 *	mediante la cual se almacena una matriz dispersa según el formato
 *	de empaquetado por columnas.
 */

/** \struct sp_packcol
 *  \brief Estructura de matriz dispersa empaquetada por columna
 *
 *  Almacena una matriz dispersa en memoria utilizando el formato simple por coordenadas.
 *  Por más información acerca de este formato, vea la sección 5.1 del paper de Stewart
 *  (ver las \ref refsec "referencias").
 */
typedef struct {
	unsigned int nrow;	/**< \brief Cantidad de filas de la matriz */
	unsigned int ncol;	/**< \brief Cantidad de columnas de la matriz */
	unsigned int nnz;	/**< \brief Cantidad de elementos no cero */
	unsigned int *colp;	/**< \brief Arreglo de punteros a los inicios de las columnas */
	unsigned int *rx;	/**< \brief Arreglo de indices de fila */
	double *val;		/**< \brief Arreglo de valores por debajo de la diagonal */
} sp_packcol;

sp_packcol* coord2packcol(sp_coord* mat);

sp_packcol* coord2packcol_symmetric(sp_coord* mat);

void sp_imprimir_packcol(FILE *fp, sp_packcol *mat);

int row_traversal_packcol(sp_packcol *A, int *i, int *j, int *posij);

void save_packcol_symmetric(FILE *fp, sp_packcol *A);

void save_packcol(FILE *fp, sp_packcol *A);

void free_packcol(sp_packcol *A);

#endif

