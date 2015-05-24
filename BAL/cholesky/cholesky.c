#include <stdlib.h>
#include <math.h>
#include "cholesky.h"
#include "../sparse/sp_packcol.h"
#include "../utils.h"

/** \file cholesky.c
 *  \brief Implementación de la factorización de Cholesky para matrices dispersas.
 *
 *  Este archivo contiene la implementación de la factorización de Cholesky, optimizado
 *  para la estructura de matrices dispersas empaquetadas por columna.
 *
 *  Esta implementación realiza los siguientes pasos:
 *  <UL>
 *   <LI>Construcción de árbol de eliminación (cálculo del número máximo de elementos no cero de la factorización)</LI>
 *   <LI>Factorización simbólica</LI>
 *   <LI>Factorización numérica</LI>
 *  </UL>
 *
 *  Para obtener el mejor rendimiento de este algoritmo, se recomienda preprocesar la matriz
 *  a factorizar con algún algoritmo de reordenamiento, para así reducir el <em>fill-in</em> producido.
 */

/** \fn int *elimination_tree(sp_packcol *A, int *nnz)
 *  \brief Calcula el árbol de eliminación de la matriz simétrica A
 *
 *  \param A ENTRADA: Matriz simétrica dispersa empaquetada por columna a procesar
 *  \param nnz SALIDA: Cantidad de elementos no cero en la factorización de Cholesky
 *  \return Estructura de padres del árbol de eliminación
 *
 *  Esta función calcula el árbol de eliminación de la matriz A,
 *  devuelve la estructura de padres del árbol y la cantidad de
 *  elementos no cero de la factorización de Cholesky.
 *
 *  \note
 *  Este paso es un paso previo a la factorización simbólica.
 *
 *  \note
 *  Esta implementación está basada en el algoritmo descrito en la sección 7.2 del paper de Stewart
 *  (ver las \ref refsec "referencias").
 */
int *elimination_tree(sp_packcol *A, int *nnz)
{
	int *touched, *parent;
	int ix, i, j, posij, js;

	/* Inicialización */
	*nnz = 0;

	if (A->nrow != A->ncol) {
		BAL_ERROR("La matriz debe ser cuadrada");
		return NULL;
	}

	/* Inicializa estructuras */
	touched = (int*)malloc(sizeof(int) * A->ncol);
	parent = (int*)malloc(sizeof(int) * A->ncol);
	for (ix = 0; ix < A->ncol; ++ix)
		touched[ix] = parent[ix] = -1;

	/* Recorre las filas de A */
	i = -2;
	row_traversal_packcol(A, &i, &j, &posij);
	for (ix = 0; ix < A->nrow; ++ix) {
		while (row_traversal_packcol(A, &i, &j, &posij) != -1) {
			if (i == j) {
				/* Procesar elemento de la diagonal */
				*nnz += 1;
				touched[j] = i;
			}
			else {
				/* Elemento no en la diagonal. Buscar el arbol */
				js = j;
				while (touched[js] != i) {
					touched[js] = i;
					*nnz += 1;

					if (parent[js] == -1) {
						parent[js] = i;
						break;
					}
					
					js = parent[js];
				}
			}
		}
	}

	/* Liberamos la memoria utilizada */
	i = -2;
	row_traversal_packcol(NULL, &i, &j, &posij);
	free(touched);

	return parent;
}

/** \fn void merge(sp_packcol *B, int j, int k, int *ma)
 *  \brief Fusiona la estructura de la columna j de B en la estructura actual de la columna k, representada por ma
 *
 *  \note
 *  Ver el algoritmo 8.2 descrito en la sección 8.2 del paper de Stewart (ver las \ref refsec "referencias").
 *
 *  \warning
 *  El pseudo-código mostrado en el paper de Stewart contiene errores.
 *  Se recomienda fuertemente comparar esta implementación (buscando los lugares marcados como corrección) con
 *  el pseudo-código de Stewart y ver las diferencias.
 */
void merge(sp_packcol *B, int j, int k, int *ma)
{
	int m, m1, i, ii;

	m = k;

	/* Itera en los elementos de la columna j de B */
	for (ii = B->colp[j]; ii < B->colp[j+1]; ++ii) { /* Corrección al algoritmo de Stewart */
		i = B->rx[ii];

		/* Corrección al algoritmo de Stewart */
		if (i-j < 1)
			continue;

		/* Busca m y m1 con m < i <= m1 */
		m1 = m;
		while (i > m1) {
			m = m1;
			m1 = ma[m];
		}

		if (i != m1) {
			/* Insertar i en ma */
			ma[m] = i;
			ma[i] = m1;
		}

		m = i;
	}
}

/** \fn void make_column(int k, int *ma, sp_packcol *L)
 *  \brief Arma la k-esima columna de L
 *
 *  \param k Columna a contruir
 *  \param ma Estructura de la columna a contruir
 *  \param L Matriz dispersa donde construir la columna
 *
 *  Esta función toma la estructura de la columna k, contenida en ma
 *  y la transfiere a la k-esima columna de L.
 *  Además reinicializa ma.
 *
 *  \note
 *  Ver el algoritmo 8.3 descrito en la sección 8.2 del paper de Stewart (ver las \ref refsec "referencias").
 */
void make_column(int k, int *ma, sp_packcol *L)
{
	int ii, m, mt;

	if (k == 0)
		L->colp[0] = 0;

	ii = L->colp[k];
	m = k;

	while (m < L->ncol) {
		L->rx[ii] = m;
		++ii;
		mt = ma[m];
		ma[m] = L->ncol;
		m = mt;
	}

	L->colp[k+1] = ii;
}

/** \fn sp_packcol *symbolic_factorization(sp_packcol *A);
 *	\brief Factorización simbólica de la matriz A
 *
 *	\param A Matriz a factorizar
 *	\return La factorización simbólica de la matriz A
 *
 *	\note
 *	Ver el algoritmo 8.1 descrito en la sección 8.2 del paper de Stewart (ver las \ref refsec "referencias").
 *
 *  \warning
 *  El pseudo-código mostrado en el paper de Stewart contiene errores.
 *  Se recomienda fuertemente comparar esta implementación (buscando los lugares marcados como corrección) con
 *  el pseudo-código de Stewart y ver las diferencias.
 */
sp_packcol *symbolic_factorization(sp_packcol *A)
{
	int nnz;				/* Cantidad de elementos no cero */
	int *parents;			/* Arbol de eliminacion */
	int *bs;				/* Hijos por nodo ("Baby Sitter") */
	int *ma;				/* "Merge array" */
	sp_packcol *L;			/* Resultado de la factorizacion simbolica */
	int i, j, k, jt;

	if (A->nrow != A->ncol) {
		BAL_ERROR("La matriz debe ser cuadrada");
		return NULL;
	}

	/* Calcula la cantidad de elementos no cero */
	parents = elimination_tree(A, &nnz);
	free(parents);

	/* Inicializacion */
	L = (sp_packcol*)malloc(sizeof(sp_packcol));
	L->nrow = A->nrow;
	L->ncol = A->ncol;
	L->nnz = (unsigned)nnz;
	L->colp = (unsigned int*)malloc(sizeof(unsigned int) * (A->ncol+1));
	L->rx = (unsigned int*)malloc(sizeof(unsigned int) * nnz);
	L->val = (double*)malloc(sizeof(double) * nnz);
	for (i = 0; i <= A->ncol; ++i)
		L->colp[i] = 0;
	for (i = 0; i < nnz; ++i) {
		L->rx[i] = 0;
		L->val[i] = 0;
	}

	bs = (int*)malloc(sizeof(int) * A->ncol);
	ma = (int*)malloc(sizeof(int) * A->ncol);
	for (i = 0; i < A->ncol; ++i) {
		bs[i] = -1;
		ma[i] = A->ncol;
	}

	/* Itera en las columnas de A */
	for (k = 0; k < A->ncol; ++k) {
		/* Computar la estructura de la k-esima columna */
		merge(A, k, k, ma);
		j = bs[k];
		bs[k] = -1; /* Corrección al algoritmo de Stewart */
		while (j != -1) {
			merge(L, j, k, ma);
			jt = bs[j];
			bs[j] = -1;
			j = jt;
		}

		/* Establecer la k-esima columna en L */
		make_column(k, ma, L);

		/* Actualizar bs */
		if (k != j) {
			j = L->rx[L->colp[k] + 1];		/* j es el padre de k */
			while (j != -1) {
				jt = j;
				j = bs[j];
			}
			bs[jt] = k;
		}
	}

	/* Libera memoria auxiliar */
	free(bs);
	free(ma);

	return L;
}

/** \fn void numerical_factorization(sp_packcol *A, sp_packcol *L)
 *	\brief Sobreescribe \c L con la factorización de Cholesky de A
 *
 *	\param A Matriz a factorizar con el algoritmo de Cholesky
 *	\param L Matriz pre inicializada donde guardar la factorización
 *
 *	\note
 *	La pre inicialización de la matriz \c L se realiza con la función symbolic_factorization().
 *
 *	\note
 *  Ver el algoritmo 9.1 descrito en la sección 9.1 del paper de Stewart (ver las \ref refsec "referencias").
 */
void numerical_factorization(sp_packcol *A, sp_packcol *L)
{
	int ii, i, j, poskj, kx, k;
	double *accum, Lkj, Lkkinv;

	accum = (double*)malloc(sizeof(double) * L->ncol);

	k = -2;
	row_traversal_packcol(L, &k, &j, &poskj);
	for (kx = 0; kx < L->ncol; ++kx) {	/* Procesa la columna k */
		while (row_traversal_packcol(L, &k, &j, &poskj) != -1) {
			if (j == k) { /* Inicializar accum */
				for (ii = L->colp[k]; ii < L->colp[k+1]; ++ii)
					accum[L->rx[ii]] = 0;
				for (ii = A->colp[k]; ii < A->colp[k+1]; ++ii)
					accum[A->rx[ii]] = A->val[ii];
			}
			else { /* Restar L[k:n,j] de L[k:n,k] */
				Lkj = L->val[poskj];
				for (ii = poskj; ii < L->colp[j+1]; ++ii) {
					i = L->rx[ii];
					accum[i] -= ( Lkj * L->val[ii] );
				}
			}
		}

		/* Mueve L[k:n,k] de accum a L, ajustando sus componentes */
		for (ii = L->colp[k]; ii < L->colp[k+1]; ++ii) { /* Corrección al algoritmo de Stewart */
			i = L->rx[ii];
			if (i == k) {
				L->val[ii] = sqrt(accum[i]);
				Lkkinv = 1 / L->val[ii];
			}
			else
				L->val[ii] = Lkkinv * accum[i];
		}
	}

	/* Libera memoria auxiliar */
	k = -2;
	row_traversal_packcol(NULL, &k, &j, &poskj);
	free(accum);
}

/** \fn void cholesky_Lsolver(sp_packcol *L, double *b)
 *	\brief Sobrescribe \c b con la solución del sistema \f$Ly=b\f$
 *
 *	\param L Matriz <TT>nxn</TT> triangular inferior, factorización de Cholesky
 *	\param b Vector de largo \c n
 *
 *	Al final de la ejecución el vector \c b es igual al vector \c y tal que \f$Ly=b\f$
 *
 *	\note
 *	Ver el algoritmo 9.2 descrito en la sección 9.2 del paper de Stewart (ver las \ref refsec "referencias")
 */
void cholesky_Lsolver(sp_packcol *L, double *b)
{
	int ii, i, j;

	for (j = 0; j < L->ncol; ++j) {
		b[j] /= L->val[L->colp[j]];

		for (ii = L->colp[j]+1; ii < L->colp[j+1]; ++ii) {
			i = L->rx[ii];
			b[i] -= ( b[j] * L->val[ii] );
		}
	}
}

/** \fn void cholesky_LTsolver(sp_packcol *L, double *b);
 *	\brief Sobreescribe \c b con la solución de \f$L^Tx=b\f$
 *
 *	\param L Matriz <TT>nxn</TT >triangular inferior, factorización de Cholesky
 *	\param b Vector de largo \c n
 *
 *	Al final de la ejecución el vector \c b es igual al vector \c y tal que \f$L^Ty=b\f$
 *
 *	\note
 *	Ver el algoritmo 9.3 descrito en la sección 9.2 del paper de Stewart (ver las \ref refsec "referencias")
 */
void cholesky_LTsolver(sp_packcol *L, double *b)
{
	int ii, i, j;

	for (j = L->ncol-1; j >= 0; --j) {
		for (ii = L->colp[j]+1; ii < L->colp[j+1]; ++ii) {
			i = L->rx[ii];
			b[j] -= ( b[i] * L->val[ii] );
		}
		b[j] /= L->val[L->colp[j]];
	}
}

/** \fn void cholesky_solver(sp_packcol *A, double *b);
 *	\brief Resuelve un sistema lineal mediante el algoritmo de Cholesky optimizado para matrices dispersas.
 *
 *	\param A Matriz del sistema lineal, simétrica definida positiva, dispersa empaquetada por columna.
 *	\param b ENTRADA/SALIDA: Vector a igualar mediante <TT>Ax</TT>
 *
 *	Esta función calcula un vetor \c x tal que \f$Ax=b\f$ y lo guarda en el mismo vector \c b.
 *	Esta implementación está completamente basada en el formato de matriz dispersa empaquetado por columna,
 *	para mejorar el desempeño de la aplicación.
 *
 *	La forma de proceder del algoritmo es la siguiente:
 *	<OL>
 *	 <LI>Se realiza la factorización simbólica de \c A</LI>
 *	 <LI>Se realiza la factorización numérica de \c A</LI>
 *	 <LI>Se resuelven los sistemas triangulares \f$Ly=b\f$ y luego \f$L^Tx=y\f$</LI>
 *	</OL>
 */
void cholesky_solver(sp_packcol *A, double *b)
{
	sp_packcol *L;

	if (A->nrow != A->ncol) {
		/* Habría que verificar además que es simétrica y definida positiva */
		BAL_ERROR("La matriz no es cuadrada");
		return;
	}

	L = symbolic_factorization(A);
	numerical_factorization(A, L);
	cholesky_Lsolver(L, b);
	cholesky_LTsolver(L, b);
}

