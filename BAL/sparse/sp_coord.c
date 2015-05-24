#include <stdlib.h>
#include <stdio.h>
#include "sp_coord.h"
#include "../utils.h"

/** \file sp_coord.c
 *	\brief Archivo de implementación para matriz dispersa, formato simple.
 *
 *	Este archivo contiene la implementación de las funciones de utilidad
 *	para el formato de matriz dispersa simple.
 */

/** \fn sp_coord* mat2coord(int n, int m, double **mat)
 *	\brief Genera la matriz dispersa equivalente a la matriz completa mat nxm.
 *	\param n Cantidad total de filas en mat
 *	\param m Cantidad total de columnas en mat
 *	\param mat Matriz completa en el formato clásico de C.
 */
sp_coord* mat2coord(int n, int m, double **mat)
{
	unsigned int i, j, count;
	sp_coord *sp;

	count = 0;
	for(i=0; i < n; ++i) {
		for(j=0; j < m; ++j) {
			if (mat[i][j] != 0)
				++count;
		}
	}

	sp = (sp_coord*)malloc(sizeof(sp_coord));
	sp->nrow = n;
	sp->ncol = m;
	sp->nnz = count;
	sp->rx = NULL;
	sp->cx = NULL;
	sp->val = NULL;

	if (count > 0) {
		sp->rx = (unsigned int*)malloc(sizeof(unsigned int) * count);
		sp->cx = (unsigned int*)malloc(sizeof(unsigned int) * count);
		sp->val = (double*)malloc(sizeof(double) * count);

		count = 0;
		for(i=0; i < n; ++i) {
			for(j=0; j < m; ++j) {
				if (mat[i][j] != 0) {
					sp->rx[count] = i;
					sp->cx[count] = j;
					sp->val[count] = mat[i][j];
					++count;
				}
			}
		}
	}

	return sp;
}

/** \fn sp_coord* load_coord(FILE *fp);
 *	\brief Lee una matriz en formato simple por coordenadas desde archivo
 *
 *  \param fp Archivo del cual leer la matriz
 *  \return La matriz leída en formato simple por coordenadas
 *
 *	Lee el archivo \c fp en busca de una definición de matriz en formato simple por coordenadas.
 *	En caso de encontrarlo, construye la estructura en memoria y la devuelve al finalizar.
 *
 *	El formato buscado es el siguiente:
 *	<OL>
 *	  <LI>El primer número leído indica la cantidad de filas de la matriz (\c nrow)</LI>
 *	  <LI>El segundo número leído indica la cantidad de columnas de la matriz (\c ncol)</LI>
 *	  <LI>El tercer número leído indica la cantidad de entradas no cero de la matriz (\c nnz)</LI>
 *	  <LI>Luego trata de leer \c nnz números enteros, que representan los índices de fila donde hay elementos no cero (\c rx)</LI>
 *	  <LI>Luego trata de leer \c nnz números enteros, que representan los índices de columna donde hay elementos no cero (\c cx)</LI>
 *	  <LI>Luego trata de leer \c nnz números reales, que representan los índices las entradas no cero de la matriz (\c val)</LI>
 *	</OL>
 *	Todos los números pueden estar separados por cualquier cantidad de espacios, tabulaciones o fines de línea.
 *
 *	\note
 *	La función save_coord() genera el formato esperado por esta función.
 */
sp_coord* load_coord(FILE *fp)
{
	int x, i;
	float y;
	sp_coord *m;

	m = (sp_coord*)malloc(sizeof(sp_coord));

	if (fscanf(fp, " %d", &x) != 1) {
		BAL_ERROR("No se pudo leer la cantidad de filas");
		free(m);
		return NULL;
	}
	m->nrow = x;

	if (fscanf(fp, " %d", &x) != 1) {
		BAL_ERROR("No se pudo leer la cantidad de columnas");
		free(m);
		return NULL;
	}
	m->ncol = x;

	if (fscanf(fp, " %d", &x) != 1) {
		BAL_ERROR("No se pudo leer la cantidad de elementos no cero");
		free(m);
		return NULL;
	}
	m->nnz = x;

	m->rx = (unsigned int*)malloc(sizeof(unsigned int) * x);
	m->cx = (unsigned int*)malloc(sizeof(unsigned int) * x);
	m->val = (double*)malloc(sizeof(double) * x);

	/* Lee los índices de fila */
	for (i=0; i < m->nnz; ++i) {
		if (fscanf(fp, " %d", &x) != 1) {
			BAL_ERROR("No se pudo leer uno de los índices de fila");
			free_coord(m);
			return NULL;
		}
		m->rx[i] = (unsigned int)x;
	}

	/* Lee los índices de columna */
	for (i=0; i < m->nnz; ++i) {
		if (fscanf(fp, " %d", &x) != 1) {
			BAL_ERROR("No se pudo leer uno de los índices de columna");
			free_coord(m);
			return NULL;
		}
		m->cx[i] = (unsigned int)x;
	}

	/* Lee las entradas de la matriz */
	for (i=0; i < m->nnz; ++i) {
		if (fscanf(fp, " %f", &y) != 1) {
			BAL_ERROR("No se pudo leer una de las entradas de la matriz");
			free_coord(m);
			return NULL;
		}
		m->val[i] = (double)y;
	}

	return m;
}

/** \fn void save_coord(FILE *fp, sp_coord *A);
 *	\brief Escribe en \c fp la matriz \c A en un formato entendible por load_coord()
 *
 *  Esta función es útil para respaldar matrices en formato simple por coordenadas.
 *  Una matriz guardada mediante esta función puede ser cargada nuevamente mediante load_coord().
 */
void save_coord(FILE *fp, sp_coord *A)
{
	unsigned int i;

	fprintf(fp, "%d\n", A->nrow);
	fprintf(fp, "%d\n", A->ncol);
	fprintf(fp, "%d\n", A->nnz);

	for (i=0; i < A->nnz; ++i)
		fprintf(fp, " %d", A->rx[i]);
	fprintf(fp, "\n");

	for (i=0; i < A->nnz; ++i)
		fprintf(fp, " %d", A->cx[i]);
	fprintf(fp, "\n");

	for (i=0; i < A->nnz; ++i)
		fprintf(fp, " %g", A->val[i]);
	fprintf(fp, "\n");
}

/** \fn void sp_imprimir_coord(FILE *fp, sp_coord *mat);
 *  \brief Imprime la matriz guardada en formato simple por coordenadas en fp
 *
 *  \param fp Archivo en el cual se imprimirá la matriz
 *  \param mat Matriz a imprimir en formato simple por coordenadas
 */
void sp_imprimir_coord(FILE *fp, sp_coord *mat)
{
	int i;

	if (mat == NULL) {
		fprintf(fp, "Matriz nula\n");
		return;
	}

	fprintf(fp, "Cantidad de filas: %d\n", mat->nrow);
	fprintf(fp, "Cantidad de columnas: %d\n", mat->ncol);
	fprintf(fp, "Cantidad de elementos no cero: %d\n", mat->nnz);

	for(i=0; i < mat->nnz; ++i) {
		fprintf(fp, "(%d) [%d,%d] = %g\n", i, mat->rx[i], mat->cx[i], mat->val[i]);
	}
}

/** \fn void free_coord(sp_coord *A);
 *	\brief Borra toda la memoria reservada por la matriz A
 *
 *  Esta función libera toda la memoria reservada por la estructura de datos sp_coord.
 */
void free_coord(sp_coord *A)
{
	free(A->rx);
	free(A->cx);
	free(A->val);
	free(A);
}

