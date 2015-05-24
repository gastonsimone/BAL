#include <stdlib.h>
#include <stdio.h>
#include "sp_packcol.h"
#include "sp_coord.h"
#include "../utils.h"

/** \file sp_packcol.c
 *	\brief Archivo de implementación para formato de matriz dispersa empaquetado por columnas
 *
 *	Este archivo contiene la implementación de las funciones de utilidad
 *	para el formato de matriz dispersa empaquetado por columnas.
 */

/** \fn sp_packcol* coord2packcol(sp_coord* mat);
 *	\brief Genera una instancia de la matriz mat en formato empaquetado por columna
 *	\param mat Matriz dispersa en formato simple por coordenadas
 */
sp_packcol* coord2packcol(sp_coord* mat)
{
	int i, j, col;
	sp_packcol* packcol = (sp_packcol*)malloc(sizeof(sp_packcol));

	packcol->nrow = mat->nrow;
	packcol->ncol = mat->ncol;
	packcol->nnz = mat->nnz;

	if (packcol->nnz == 0) {
		packcol->colp = NULL;
		packcol->rx = NULL;
		packcol->val = NULL;
	}
	else {
		packcol->colp = (unsigned int*)malloc(sizeof(unsigned int) * (packcol->ncol+1));
		packcol->rx = (unsigned int*)malloc(sizeof(unsigned int) * packcol->nnz);
		packcol->val = (double*)malloc(sizeof(double) * packcol->nnz);

		j = 0;
		for(col=0; col < packcol->ncol; ++col) {	/*< Para cada columna col */
			packcol->colp[col] = j;					/*< El 1er elem de la columna col esta en la posicion j de val */
			for(i=0; i < mat->nnz; ++i) {			/*< Recorro los elementos no-cero de mat */
				if (mat->cx[i] == col) {			/*< Si es un elemento de la columna col */
					packcol->val[j] = mat->val[i];	/*< Guardo el valor */
					packcol->rx[j] = mat->rx[i];	/*< Guardo el numero de fila */
					++j;
				}
			}
		}
		packcol->colp[col] = j;					/*< Elemento extra para indicar el fin de la ultima columna */
	}

	return packcol;
}

/** \fn sp_packcol* coord2packcol_symmetric(sp_coord* mat);
 *	\brief Genera una instancia de la matriz mat en formato empaquetado por columna para matrices simétricas
 *	\param mat Matriz dispersa en formato simple por coordenadas
 *
 *	En el caso de que mat sea una matriz simétrica, no es necesario guardar todos los elementos no cero.
 *	Basta con guardar los elementos no cero de una de las triangulares.
 *
 *	Esta función genera una representación de la matriz mat en formato empaquetado por columna, guardando en
 *	ella solo los elementos que están en la diagonal mayor y por debajo de ella, economizando memoria y sin
 *	perder información.
 *
 *	Por supuesto, esta función debe ser utilizada solo cuando mat es simétrica y se desea tener la ganancia de memoria.
 *	Las rutinas que utilicen la estructura generada por esta función deberán tener en cuenta sus características
 *	para realizar las tareas de forma correcta.
 */
sp_packcol* coord2packcol_symmetric(sp_coord* mat)
{
	int i, j, col, nnz;
	sp_packcol* packcol = (sp_packcol*)malloc(sizeof(sp_packcol));

	if (mat->nrow != mat->ncol) {
		BAL_ERROR("La matriz debe ser cuadrada para poder ser simetrica.");
		return NULL;
	}

	packcol->nrow = mat->nrow;
	packcol->ncol = mat->ncol;

	if (mat->nnz == 0) {
		packcol->colp = NULL;
		packcol->rx = NULL;
		packcol->val = NULL;
	}
	else {
		/* Calcula la cantidad de elementos no cero debajo y en la diagonal */
		nnz = 0;
		for(i=0; i < mat->nnz; ++i) {
			if (mat->rx[i] >= mat->cx[i])
				++nnz;
		}

		packcol->nnz = nnz;
		packcol->colp = (unsigned int*)malloc(sizeof(unsigned int) * (packcol->ncol+1));
		packcol->rx = (unsigned int*)malloc(sizeof(unsigned int) * nnz);
		packcol->val = (double*)malloc(sizeof(double) * nnz);

		j = 0;
		for(col=0; col < packcol->ncol; ++col) {						/*< Para cada columna col */
			packcol->colp[col] = j;										/*< El 1er elem de la columna col esta en la posicion j de val */
			for(i=0; i < mat->nnz; ++i) {								/*< Recorro los elementos no-cero de mat */
				if (mat->rx[i] >= mat->cx[i] && mat->cx[i] == col) {	/*< Si es un elemento de la columna col */
					packcol->val[j] = mat->val[i];						/*< Guardo el valor */
					packcol->rx[j] = mat->rx[i];						/*< Guardo el numero de fila */
					++j;
				}
			}
		}
		packcol->colp[col] = j;					/*< Elemento extra para indicar el fin de la ultima columna */
	}

	return packcol;
}

/** \fn void sp_imprimir_packcol(FILE *fp, sp_packcol *mat);
 *  \brief Imprime la matriz guardada en formato empaquetado por columna en \c fp
 *
 *  \param fp Archivo en el cual se imprimirá la matriz
 *  \param mat Matriz a imprimir en formato empaquetado por columna
 *
 *  <B>NOTA:</B> Ver el código (5.4) en el paper de Stewart (vea las referencias).
 */
void sp_imprimir_packcol(FILE *fp, sp_packcol *mat)
{
	int i, j;

	if (mat == NULL) {
		fprintf(fp, "Matriz nula.\n");
		return;
	}

	fprintf(fp, "Cantidad de filas: %d\n", mat->nrow);
	fprintf(fp, "Cantidad de columnas: %d\n", mat->ncol);
	fprintf(fp, "Cantidad de elementos no cero: %d\n", mat->nnz);

	fprintf(fp, "Inicios de columnas:\n");
	for(i=0; i <= mat->ncol; ++i) {
		fprintf(fp, "%d ", mat->colp[i]);
	}
	fprintf(fp, "\n");

	fprintf(fp, "Indices de filas:\n");
	for(i=0; i < mat->nnz; ++i) {
		fprintf(fp, "%d ", mat->rx[i]);
	}
	fprintf(fp, "\n");

	fprintf(fp, "Valores:\n");
	for(i=0; i < mat->nnz; ++i) {
		fprintf(fp, "%g ", mat->val[i]);
	}
	fprintf(fp, "\n");

	fprintf(fp, "Salida indexada (por columna):\n");
	i=0;
	for(j=0; j < mat->ncol; ++j) {
		for (i = mat->colp[j]; i < mat->colp[j+1]; ++i) {
			fprintf(fp, "(%d) [%d,%d] = %g\n", i, mat->rx[i], j, mat->val[i]);
		}
	}
}

/** \fn int row_traversal_packcol(sp_packcol *A, int *i, int *j, int *posij);
 *  \brief Implementa un mecanismo eficiente para recorrer por filas  una matriz dispersa empaquetada por columnas.
 *
 *  \param A ENTRADA: Matriz a recorrer por filas en formato empaquetado por columna con mejora por simetría.
 *  \param i ENTRADA/SALIDA: Si <TT>i = -2</TT>, la rutina es inicializada. En otro caso, \c i guarda el índice de la fila leída.
 *  \param j SALIDA: Guarda el índice de la columna leída.
 *  \param posij SALIDA: Indica la posición de <TT>A[i.j]</TT> en <TT>A->val</TT>.
 *  \return -1 si se llegó al final de una línea, \c j en caso contrario.
 *
 *  Dada la forma de guardar los datos en memoria de la estructura sp_packcol, no es trivial recorrer la matriz
 *  por filas de forma eficiente. Esta función implementa un mecanismo para ir obteniendo los valores de una sp_packcol
 *  por fila.
 *
 *  \warning
 *  Esté método solo es útil para matrices simétricas guardadas tal como lo hace coord2packcol_symmetric().
 *
 *  \warning
 *  No se debe cambiar el valor de las variables \c i y \c j mientras se está recorriendo una matriz utilizando esta función.
 *
 *  \warning
 *  Esta función devuelve todos los elementos de una fila \c i antes de devolver los elementos de la fila <TT>i+1</TT>,
 *  pero los elementos de una misma fila no son devueltos en ningún orden en particular (por ejemplo, ordenados por columna,
 *  que sería lo más natural). Más precisamente, la rutina siempre devuelve primero el elemento de la diagonal mayor <TT>(i,i)</TT>,
 *  (en caso que no sea cero), pero el resto de los elementos no tienen un orden particular definido.
 *
 *  \note
 *  Este algoritmo brinda un método de recorrido por fila que presenta un tiempo de ejecución de <em>O(A.nnz)</em> y
 *  conlleva un costo extra en memoria de <em>2n</em> variables de tipo entero, siendo \c n la cantidad de filas y columnas de \c A.
 *
 *  \note
 *  Esta función está basada en la descripción de la sección 5.3 del paper de Stewart (Ver las \ref refsec "referencias").
 *  Puede ver ese documento o el juego de rutinas de prueba de BAL por un ejemplo de cómo utilizar esta rutina.
 *
 *  \note
 *  Esta rutina utiliza memoria dinámica para trabajar. Puede llamar a esta función con <TT>A = NULL</TT> y <TT>i = -2</TT>
 *  para liberar la memoria utilizada. Note que esto es diferente a llamar la rutina para que se inicialice, en la que la misma
 *  reserva memoria para trabajar.
 */
int row_traversal_packcol(sp_packcol *A, int *i, int *j, int *posij)
{
	static int *link = NULL;
	static int *pos = NULL;
	static int nextj = -1;
	int x, nextdown, id;

	if (*i == -2) { /* Inicializacion */
		if (link != NULL)
			free(link);

		if (pos != NULL)
			free(pos);

		if (A == NULL) {
			link = pos = NULL;
			return -1;
		}

		link = (int*)malloc(sizeof(int) * A->ncol);
		pos = (int*)malloc(sizeof(int) * A->ncol);
		for (x = 0; x < A->ncol; ++x)
			link[x] = pos[x] = -1;

		*i = *j = -1;
		return -1;
	}

	if (*j == -1) { /* Preparamos la fila i */
		*i += 1;
		*j = *i;
		*posij = A->colp[*i];
	}
	else { /* Obtener el siguiente elemento de la fila i */
		*j = nextj;

		if (*j == -1)
			return *j;	/* Fin de fila */

		*posij = pos[*j];
	}

	nextj = link[*j];
	link[*j] = -1;
	nextdown = *posij + 1;

	if (nextdown < A->colp[*j + 1]) {	/* Hay un elemento en la columna j, recordarlo */
		pos[*j] = nextdown;
		id = A->rx[nextdown];
		link[*j] = link[id];
		link[id] = *j;
	}

	return *j;
}

/** \fn void save_packcol_symmetric(FILE *fp, sp_packcol *A);
 *	\brief Imprime la matriz simétrica empaquetada por columna en formato matlab en el archivo fp.
 *
 *	\param fp Puntero a archivo donde imprimir la matriz A.
 *	\param A Matriz simétrica empaquetada por columna a imprimir en formato matlab.
 *
 *	Esta función es útil para respaldar matrices.
 *	
 *	\note
 *	Esta función solo funciona para matrices empaquetadas por columna que fueron guardadas
 *	con la mejora para matrices simétricas, tal como lo hace la función coord2packcol_symmetric.
 *
 *	\warning
 *	La salida produce solo la triangular inferior de la matriz, para una salida
 *	completa, utilice save_packcol().
 */
void save_packcol_symmetric(FILE *fp, sp_packcol *A)
{
	int x, y, i, j, posij;
	double *fila;

	fila = (double*)malloc(sizeof(double) * A->ncol);

	fprintf(fp, "[\n");

	i = -2;
	row_traversal_packcol(A, &i, &j, &posij);
	for (x = 0; x < A->nrow; ++x) {	/* Por cada fila */

		for (y=0; y < A->ncol; ++y)
			fila[y] = 0;

		while(row_traversal_packcol(A, &i, &j, &posij) != -1)
			fila[j] = A->val[posij];

		for (y=0; y < A->ncol; ++y)
			fprintf(fp, " %g", fila[y]);

		if (x+1 < A->nrow)
			fprintf(fp, ";\n");
		else
			fprintf(fp, "\n");
	}

	fprintf(fp, "]\n");

	free(fila);
}

/** \fn void save_packcol(FILE *fp, sp_packcol *A);
 *	\brief Imprime la matriz \c A en formato matlab en el archivo \c fp.
 *
 *	\param fp Puntero a archivo donde imprimir la matriz A.
 *	\param A Matriz simétrica empaquetada por columna a imprimir en formato matlab.
 *
 *	Esta función es útil para respaldar matrices.
 */
void save_packcol(FILE *fp, sp_packcol *A)
{
	unsigned int i, j, k, encontrado;

	fprintf(fp, "[\n");
	for (i=0; i < A->nrow; ++i) {			/* Por cada fila */
		for (j=0; j < A->ncol; ++j) {		/* Por cada columna */
			/* Busca la entrada (i,j) en la columna j de A */
			encontrado = 0;
			for (k=A->colp[j]; k < A->colp[j+1]; ++k) {
				if (A->rx[k] == i) {
					encontrado = 1;
					fprintf(fp, " %g", A->val[k]);
					break;
				}
			}
			if (!encontrado)
				fprintf(fp, " 0");
		}
		if (i+1 < A->nrow)
			fprintf(fp, ";\n");
		else
			fprintf(fp, "\n");
	}
	fprintf(fp, "]\n");
}

/** \fn void free_packcol(sp_packcol *A);
 *	\brief Borra toda la memoria reservada por la matriz A
 *
 *  Esta función libera toda la memoria reservada por las estructura de datos sp_packcol.
 */
void free_packcol(sp_packcol *A)
{
	free(A->colp);
	free(A->rx);
	free(A->val);
	free(A);
}

