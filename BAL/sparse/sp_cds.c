#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include "sp_cds.h"
#include "../utils.h"

/** \file sp_cds.c
 *	\brief Archivo de implementación para matriz dispersa, formato simple.
 *
 *	Este archivo contiene la implementación de las funciones de utilidad
 *	para el formato de matriz dispersa simple.
 */

/** \fn sp_cds* coord2cds(sp_coord *mat);
 *	\brief Genera la matriz en formato CDS equivalente a la matriz mat en formato simple.
 *	\param mat Matriz dispersa en formato simple.
 *	\return Matriz equivalente en formato CDS.
 */
sp_cds* coord2cds(sp_coord *mat)
{
	unsigned int n, m, tope, ii, i, j, dxl;
	int diag, k;
	sp_cds *cds;

	cds = (sp_cds*)malloc(sizeof(sp_cds));

	cds->nrow = n = mat->nrow;
	cds->ncol = m = mat->ncol;
	cds->maxdiaglength = MIN(n, m);
	tope = m + n - 1; /* Cantidad máxima de diagonales */
	cds->dx = (int*)malloc(sizeof(int) * tope);

	/* Busca diagonales con datos */
	dxl = 0;
	for (ii=0; ii < mat->nnz; ++ii) {
		i = mat->rx[ii];
		j = mat->cx[ii];
		diag = j - i;

		if (binary_search(cds->dx, dxl, diag) == -1) {
			insert_sorted(cds->dx, dxl - 1, diag);
			++dxl;
		}
	}

	/* Inicializa cds->val */
	cds->ndiag = dxl;
	cds->val = (double**)malloc(sizeof(double*) * dxl);
	for (i=0; i < dxl; ++i) {
		cds->val[i] = (double*)malloc(sizeof(double) * cds->maxdiaglength);
		for (j=0; j < cds->maxdiaglength; ++j)
			cds->val[i][j] = 0;
	}

	/* Carga los valores */
	for (ii=0; ii < mat->nnz; ++ii) {
		i = mat->rx[ii];
		j = mat->cx[ii];
		diag = j - i;

		k = binary_search(cds->dx, dxl, diag);
		cds->val[k][i] = mat->val[ii];
	}

	return cds;
}

/** \fn void sp_imprimir_cds(FILE *fp, sp_cds *mat);
 *  \brief Imprime la matriz guardada en formato simple por coordenadas en fp
 *
 *  \param fp Archivo en el cual se imprimirá la matriz
 *  \param mat Matriz a imprimir en formato CDS
 */
void sp_imprimir_cds(FILE *fp, sp_cds *mat)
{
	unsigned int i, j;

	fprintf(fp, "Cantidad de filas: %d\n", mat->nrow);
	fprintf(fp, "Cantidad de columnas: %d\n", mat->ncol);
	fprintf(fp, "Cantidad de diagonales no-cero: %d\n", mat->ndiag);
	fprintf(fp, "Largo maximo de diagonal: %d\n", mat->maxdiaglength);

	fprintf(fp, "Mapeo de diagonales:");
	for (i=0; i < mat->ndiag; ++i)
		fprintf(fp, " %d", mat->dx[i]);

	fprintf(fp, "\nValores:\n");
	for (i=0; i < mat->ndiag; ++i) {
		fprintf(fp, "val[%d,0:%d] =", i, mat->maxdiaglength-1);
		for (j=0; j < mat->maxdiaglength; ++j)
			fprintf(fp, " %g", mat->val[i][j]);
		fprintf(fp, "\n");
	}
}

/** \fn void save_cds(FILE *fp, sp_cds *A);
 *	\brief Imprime la matriz \c A en formato matlab en el archivo \c fp.
 *
 *	\param fp Archivo en donde imprimir
 *	\param A Matriz a imprimir, en formato CDS
 *
 *	Esta función es útil para respaldar matrices.
 */
void save_cds(FILE *fp, sp_cds *A)
{
	unsigned int i, j;
	int diag, k;

	fprintf(fp, "[\n");

	for (i=0; i < A->nrow; ++i) {
		for (j=0; j < A->ncol; ++j) {
			diag = j - i;
			k = binary_search(A->dx, A->ndiag, diag);
			if (k != -1)
				fprintf(fp, " %g", A->val[k][i]);
			else
				fprintf(fp, " 0");
		}
		if (i+1 < A->nrow)
			fprintf(fp, ";\n");
		else
			fprintf(fp, "\n");
	}

	fprintf(fp, "]\n");
}

/** \fn void free_cds(sp_cds *A);
 *	\brief Borra toda la memoria reservada por la matriz A
 *
 *  Libera la memoria reservada por la estructura de datos sp_cds.
 */
void free_cds(sp_cds *A)
{
	unsigned int d;

	for (d=0; d < A->ndiag; ++d)
		free(A->val[d]);

	free(A->val);
	free(A->dx);
	free(A);
}

