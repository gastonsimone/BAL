#include <glib.h>
#include <stdio.h>
#include "bal.h"
#include "oper.h"
#include "sparse/sp_coord.h"
#include "sparse/sp_packcol.h"
#include "sparse/sp_cds.h"
#include "cholesky/cholesky.h"
#include "cholesky/reordenamiento.h"

/** \file bal.c
 *  \brief Biblioteca de Algebra Lineal, archivo principal
 *
 *  Este archivo implementa las funciones disponibles mediante BAL.
 */

/** \fn extern int yyparse(const char *archivo, double ***matriz, int *n, int *m);
 *	\brief Parser de matrices en formato matlab generado con bison y flex
 *
 *	\param archivo ENTRADA: Camino al archivo donde esta definida la matriz
 *	\param matriz SALIDA: Matriz leída como lista de filas. Cada fila es una lista de valores.
 *	\param n SALIDA: Cantidad de filas de la matriz leída.
 *	\param m SALIDA: Cantidad de columnas de la matriz leída.
 *
 *	<B>NOTA:</B> Esta función es de uso interno de BAL. No debería ser necesario su uso externo.
 *  Vea la función bal_cargar_matriz.
 */
extern int yyparse(const char *archivo, double ***matriz, int *n, int *m);

/** \fn int bal_cargar_matriz(const char *archivo, double ***matriz, int *n, int *m)
 *  \brief Carga una matriz en memoria desde un archivo.
 *
 *  \param archivo ENTRADA: El camino al archivo en el que esta definida la matriz
 *  \param matriz SALIDA: Puntero a la estructura de memoria donde fue alocada la matriz
 *  \param n SALIDA: Cantidad de filas
 *  \param m SALIDA: Cantidad de columnas
 *
 *  La matriz es cargada en memoria de forma "convencional".
 *  El formato esperado es el mismo formato usado por matlab para definir matrices.
 *  El parser fue implementado utilizando las herramientas bison y flex de forma combinada.
 *  La función retorna 0 si todo funciono correctamente y otro valor en caso contrario.
 */
int bal_cargar_matriz(const char *archivo, double ***matriz, int *n, int *m)
{
	if (yyparse(archivo, matriz, n, m) != 0)
		return -1;

	return 0;
}

/** \fn void bal_imprimir_matriz(FILE *fp, double **matriz, int n, int m)
 *  \brief Imprime la matriz en el archivo \c fp
 *
 */
void bal_imprimir_matriz(FILE *fp, double **matriz, int n, int m)
{
	int i, j;

	for(i=0; i < n; i++) {
		for(j=0; j < m; j++) {
			fprintf(fp, "\t%g", matriz[i][j]);
		}
		fprintf(fp, "\n");
	}
}

/** \fn sp_coord* bal_mat2coord(int n, int m, double **mat)
 *	\brief Genera la matriz dispersa equivalente a la matriz completa mat (n x m).
 *
 *	Por más información, vea mat2coord().
 */
sp_coord* bal_mat2coord(int n, int m, double **mat)
{
	return mat2coord(n, m, mat);
}

/** \fn sp_packcol* bal_coord2packcol(sp_coord* mat);
 *	\brief Genera una instancia de la matriz mat en formato empaquetado por columna
 *
 *	Por más información vea coord2packcol().
 */
sp_packcol* bal_coord2packcol(sp_coord* mat)
{
	return coord2packcol(mat);
}

/** \fn sp_packcol* bal_coord2packcol_symmetric(sp_coord* mat);
 *	\brief Genera una instancia de la matriz mat en formato empaquetado por columna para matrices simétricas
 *
 *	Por más información vea coord2packcol_symmetric().
 */
sp_packcol* bal_coord2packcol_symmetric(sp_coord* mat)
{
	return coord2packcol_symmetric(mat);
}

/** \fn void bal_imprimir_coord(FILE *fp, sp_coord *mat);
 *  \brief Imprime la matriz guardada en formato simple por coordenadas en fp
 *
 *  Por más información vea sp_imprimir_coord().
 */
void bal_imprimir_coord(FILE *fp, sp_coord *mat)
{
	sp_imprimir_coord(fp, mat);
}

/** \fn void bal_imprimir_packcol(FILE *fp, sp_packcol *mat);
 *  \brief Imprime la matriz guardada en formato empaquetado por columna en fp
 *
 *  Por más información vea sp_imprimir_packcol().
 */
void bal_imprimir_packcol(FILE *fp, sp_packcol *mat)
{
	sp_imprimir_packcol(fp, mat);
}

/** \fn void bal_mult_vec_packcol_symmetric(sp_packcol *A, double *x, double *y);
 *  \brief Multiplica una matriz simétrica empaquetada por columna por un vector.
 *
 *  Por más información vea mult_vec_packcol_symmetric().
 */
void bal_mult_vec_packcol_symmetric(sp_packcol *A, double *x, double *y)
{
	mult_vec_packcol_symmetric(A, x, y);
}

/** \fn void bal_mult_vec_packcol(sp_packcol *A, double *x, double *y);
 *  \brief Multiplica una matriz empaquetada por columna por un vector.
 *
 *  Por más información vea mult_vec_packcol().
 */
void bal_mult_vec_packcol(sp_packcol *A, double *x, double *y)
{
	mult_vec_packcol(A, x, y);
}

/** \fn int bal_row_traversal_packcol(sp_packcol *A, int *i, int *j, int *posij);
 *  \brief Implementa un mecanismo eficiente para recorrer por filas  una matriz dispersa empaquetada por columnas.
 *
 *  Por más información vea row_traversal_packcol().
 */
int bal_row_traversal_packcol(sp_packcol *A, int *i, int *j, int *posij)
{
	return row_traversal_packcol(A, i, j, posij);
}

/** \fn sp_coord* bal_load_coord(FILE *fp);
 *	\brief Escribe en \c fp la matriz \c A en un formato entendible por load_coord()
 *
 *  Por más información vea load_coord().
 */
sp_coord* bal_load_coord(FILE *fp)
{
	return load_coord(fp);
}

/** \fn void bal_save_coord(FILE *fp, sp_coord *A);
 *	\brief Escribe en \c fp la matriz \c A en un formato entendible por load_coord()
 *
 *  Por más información vea save_coord().
 */
void bal_save_coord(FILE *fp, sp_coord *A)
{
	save_coord(fp, A);
}

/** \fn void bal_save_packcol_symmetric(FILE *fp, sp_packcol *A);
 *	\brief Imprime la matriz simétrica empaquetada por columna en formato matlab en el archivo fp.
 *
 *	Por más información vea save_packcol_symmetric().
 */
void bal_save_packcol_symmetric(FILE *fp, sp_packcol *A)
{
	save_packcol_symmetric(fp, A);
}

/** \fn void bal_save_packcol(FILE *fp, sp_packcol *A);
 *	\brief Imprime la matriz \c A en formato matlab en el archivo \c fp.
 *
 *	Por más información vea save_packcol().
 */
void bal_save_packcol(FILE *fp, sp_packcol *A)
{
	save_packcol(fp, A);
}

/** \fn int *bal_elimination_tree(sp_packcol *A, int *nnz)
 *  \brief Calcula el árbol de eliminación de la matriz simétrica A
 *
 *  Por más información vea elimination_tree().
 */
int *bal_elimination_tree(sp_packcol *A, int *nnz)
{
	return elimination_tree(A, nnz);
}

/** \fn sp_packcol *bal_symbolic_factorization(sp_packcol *A);
 *	\brief Factorización simbólica de la matriz A
 *
 *	Por más información vea symbolic_factorization().
 */
sp_packcol *bal_symbolic_factorization(sp_packcol *A)
{
	return symbolic_factorization(A);
}

/** \fn void bal_numerical_factorization(sp_packcol *A, sp_packcol *L)
 *	\brief Sobreescribe \c L con la factorización de Cholesky de A
 *
 *	Por más información vea numerical_factorization().
 */
void bal_numerical_factorization(sp_packcol *A, sp_packcol *L)
{
	numerical_factorization(A, L);
}

/** \fn void bal_cholesky_Lsolver(sp_packcol *L, double *b)
 *	\brief Sobrescribe \c b con la solución del sistema \f$Ly=b\f$
 *
 *	Por más información vea cholesky_Lsolver().
 */
void bal_cholesky_Lsolver(sp_packcol *L, double *b)
{
	cholesky_Lsolver(L, b);
}

/** \fn void bal_cholesky_LTsolver(sp_packcol *L, double *b);
 *	\brief Sobreescribe \c b con la solución de \f$L^Tx=b\f$
 *
 *	Por más información vea cholesky_LTsolver().
 */
void bal_cholesky_LTsolver(sp_packcol *L, double *b)
{
	cholesky_LTsolver(L, b);
}

/** \fn void bal_cholesky_solver(sp_packcol *A, double *b);
 *	\brief Resuelve un sistema lineal mediante el algoritmo de Cholesky optimizado para matrices dispersas.
 *
 *	Por más información vea cholesky_solver().
 */
void bal_cholesky_solver(sp_packcol *A, double *b)
{
	cholesky_solver(A, b);
}

/** \fn sp_cds* bal_coord2cds(sp_coord *mat);
 *	\brief Genera la matriz en formato CDS equivalente a la matriz mat en formato simple.
 *
 *	Por más información vea coord2cds().
 */
sp_cds* bal_coord2cds(sp_coord *mat)
{
	return coord2cds(mat);
}

/** \fn void bal_imprimir_cds(FILE *fp, sp_cds *mat);
 *  \brief Imprime la matriz guardada en formato simple por coordenadas en fp
 *
 *	Por más información vea sp_imprimir_cds().
 */
void bal_imprimir_cds(FILE *fp, sp_cds *mat)
{
	sp_imprimir_cds(fp, mat);
}

/** \fn void bal_mult_vec_cds(sp_cds *A, double *x, double *y);
 *  \brief Multiplica una matriz en formato CDS por un vector.
 *
 *  Por más información ver mult_vec_cds().
 */
void bal_mult_vec_cds(sp_cds *A, double *x, double *y)
{
	mult_vec_cds(A, x, y);
}

/** \fn sp_cds* bal_mult_mat_cds(sp_cds *A, sp_cds *B);
 *  \brief Multiplica dos matrices en formato CDS.
 *
 *  Por más información ver mult_mat_cds().
 */
sp_cds* bal_mult_mat_cds(sp_cds *A, sp_cds *B)
{
	return mult_mat_cds(A, B);
}

/** \fn void bal_save_cds(FILE *fp, sp_cds *A);
 *	\brief Imprime la matriz \c A en formato matlab en el archivo \c fp.
 *
 *	Por más información vea save_cds().
 */
void bal_save_cds(FILE *fp, sp_cds *A)
{
	save_cds(fp, A);
}

/** \fn sp_packcol* bal_permutar_packcol(unsigned int *p, sp_packcol *A)
 *	\brief Aplica una permutación de columnas sobre la matriz \c A
 *
 *	Por más información vea permutar_packcol().
 */
sp_packcol* bal_permutar_packcol(unsigned int *p, sp_packcol *A)
{
	return permutar_packcol(p, A);
}

/** \fn void bal_free_coord(sp_coord *A);
 *	\brief Libera la memoria reservada por la matriz \c A
 *
 *	Por más información ver free_coord()
 */
void bal_free_coord(sp_coord *A)
{
	free_coord(A);
}

/** \fn void bal_free_packcol(sp_packcol *A);
 *	\brief Libera la memoria reservada por la matriz \c A
 *
 *	Por más información ver free_packcol()
 */
void bal_free_packcol(sp_packcol *A)
{
	free_packcol(A);
}

/** \fn void bal_free_cds(sp_cds *A);
 *	\brief Libera la memoria reservada por la matriz \c A
 *
 *	Por más información ver free_cds()
 */
void bal_free_cds(sp_cds *A)
{
	free_cds(A);
}

