#ifndef __SP_COORD_H__
#define __SP_COORD_H__

#include <stdio.h>

/** \file sp_coord.h
 *	\brief Archivo de cabecera para matriz dispersa, formato simple.
 *
 *	Este archivo contiene la definición de la estructura de datos
 *	mediante la cual se almacena una matriz dispersa según el formato
 *	simple, representación por coordenadas.
 */

/** \struct sp_coord
 *  \brief Estructura de matriz dispersa simple por coordenadas
 *
 *  Almacena una matriz dispersa en memoria utilizando el formato simple por coordenadas.
 *  Por más información acerca de este formato, vea la sección 5.1 del paper de Stewart
 *  (ver las \ref refsec "referencias").
 */
typedef struct {
	unsigned int nrow;	/**< \brief Cantidad de filas */
	unsigned int ncol;	/**< \brief Cantidad de columnas */
	unsigned int nnz;	/**< \brief Cantidad de elementos no cero */
	unsigned int *rx;	/**< \brief Arreglo de indices de fila */
	unsigned int *cx;	/**< \brief Arreglo de indices de columna */
	double *val;		/**< \brief Arreglos de valores */
} sp_coord;

sp_coord* mat2coord(int n, int m, double **mat);

sp_coord* load_coord(FILE *fp);

void save_coord(FILE *fp, sp_coord *A);

void sp_imprimir_coord(FILE *fp, sp_coord *mat);

void free_coord(sp_coord *A);

#endif

