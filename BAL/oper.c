#include <stdlib.h>
#include <glib.h>
#include "sparse/sp_packcol.h"
#include "sparse/sp_cds.h"
#include "utils.h"
#include "oper.h"

/** \file oper.c
 *  \brief Operaciones básicas, implementación
 *
 *  Este archivo contiene las implementaciones de las funciones que implementan
 *  operaciones básicas entre matrices y vectores utilizando estructuras de
 *  datos para matrices dispersas.
 */

/** \fn void mult_vec_packcol(sp_packcol *A, double *x, double *y);
 *  \brief Multiplica una matriz empaquetada por columna por un vector.
 *
 *  \param A ENTRADA: Matriz dispersa empaquetada por columna simétrica
 *  \param x ENTRADA: Vector a multiplicar
 *  \param y SALIDA: Resultado de la multiplicación
 *
 *  Esta función realiza la operación \f$y = Ax\f$ siendo \c A una matriz empaquetada por columna.
 *
 *  \note
 *  La estructura en memoria es tal como la generada por la función coord2packcol()
 *
 *  \warning
 *  Esta función no reserva memoria. El vector \c y ya debe estar inicializado en el tamaño
 *  correcto antes de llamar a esta función.
 */
void mult_vec_packcol(sp_packcol *A, double *x, double *y)
{
	int i, ii, j;

	/* Inicializa vector y */
	for (j = 0; j < A->nrow; ++j)
		y[j] = 0;

	for(j = 0; j < A->ncol; ++j) {
		for(ii = A->colp[j]; ii < A->colp[j+1]; ++ii) {
			i = A->rx[ii];
			y[i] += ( x[j] * A->val[ii] );
		}
	}
}

/** \fn void mult_vec_packcol_symmetric(sp_packcol *A, double *x, double *y);
 *  \brief Multiplica una matriz simétrica empaquetada por columna por un vector.
 *
 *  \param A Matriz dispersa empaquetada por columna simétrica
 *  \param x Vector a multiplicar
 *  \param y Resultado de la multiplicación
 *
 *  Esta función calcula la operación \f$y = Ax\f$ bajo las siguientes consideraciones:
 *  <UL>
 *   <LI>La matriz A está guardada con la mejora para matrices simétricas, tal cual lo hace la función coord2packcol_symmetric()</LI>
 *   <LI>Los vectores \c x e \c y tienen <TT>A.nrow</TT> elementos (Ver definición de la estructura sp_packcol) y ya están inicializados</LI>
 *  </UL>
 *
 *  \note
 *  Ver el algoritmo 5.1 (sección 5.2 Matrix-vector multiplication) en el paper de Stewart (vea las \ref refsec "referencias").
 *
 *  \warning
 *  La implementación sugerida por Stewart tiene un bug:
 *  el algoritmo no devuelve el resultado correcto si la diagonal mayor de \c A contiene ceros.
 *  En esta implementación se corrigió esta falla.
 *  Se sugiere comparar las dos implementaciones para ver las diferencias.
 */
void mult_vec_packcol_symmetric(sp_packcol *A, double *x, double *y)
{
	int i, ii, j, r;

	/* Inicializa vector y */
	for (j = 0; j < A->nrow; ++j)
		y[j] = 0;

	for (j = 0; j < A->ncol; ++j) {
		i = A->rx[A->colp[j]];						/* Indice de fila del 1er elem no cero de la columna j */

		if (i == j) {								/* Si el 1er no cero es la diag */
			y[j] += ( x[j] * A->val[A->colp[j]] );	/* Procesar diagonal: y_j += x_j * A_jj */
			r = 1;									/* Ignorar la diagonal en el loop interno */
		}
		else
			r = 0;									/* El 1er elem no cero no es la diagonal. No ignorarlo. */

		/* Procesa los elementos no en la diagonal utilizando la propiedad de simetria */
		for (ii = A->colp[j] + r; ii <= A->colp[j+1] - 1; ++ii) {
			i = A->rx[ii];							/* Obtiene el indice de fila */
			y[i] += ( x[j] * A->val[ii] );			/* y_i += x_j * A_ij */
			y[j] += ( x[i] * A->val[ii] );			/* y_i += x_i * A_ji */
		}
	}
}

/** \fn void mult_vec_cds(sp_cds *A, double *x, double *y);
 *  \brief Multiplica una matriz en formato CDS por un vector.
 *
 *  \param A ENTRADA: Matriz dispersa en formato CDS
 *  \param x ENTRADA: Vector a multiplicar
 *  \param y SALIDA: Resultado de la multiplicación
 *
 *  Esta función realiza la operación \f$y = Ax\f$ siendo \c A una matriz en formato CDS.
 *
 *  Teniendo en cuenta que cada elemento de una diagonal de \c A solo afecta una entrada del
 *  resultado \c y, la estrategia es recorrer la matriz por diagonales (aprovechando la estructura CDS)
 *  y, para cada elemento de la diagonal, actualizar cada entrada de \c y según cómo la afecta el elemento procesado.
 *  Luego de recorrer todas las diagonales, el vector \c y contiene el resultado esperado.
 *
 *  Dado que una fila de \c val puede tener entradas que ni siquiera representan un elemento válido en \c A,
 *  es importante ver desde dónde y hasta dónde se recorre cada fila en \c val. Aprovechando la alineación que se le da
 *  a cada diagonal dentro de la estructura (a la izquierda si es superior y a la derecha si es inferior, ver la descripción
 *  de la estructura sp_cds) el algoritmo precalcula el rango de entradas donde hay datos válidos antes de iterar en los
 *  elementos de la diagonal, es decir, antes de comenzar con el loop interior.
 *
 *  \note
 *  Es importante notar que las entradas del vector \c y se van construyendo a medida que se van recorriendo
 *  las diagonales de \c A, y no son construidas en un único paso, como es el caso del algoritmo del producto
 *  de matriz-vector clásico.
 *
 *  \note
 *  Este algoritmo ejecuta en un tiempo que está en el orden <em>O(D(A))</em>, siendo <TT>D(A)</TT> la cantidad
 *  de entradas (con valor cero o no cero) que componen todas las diagonales no vacías de \c A.
 *
 *  \warning
 *  Esta función no reserva memoria. El vector \c y ya debe estar inicializado en el tamaño
 *  correcto antes de llamar a esta función.
 */
void mult_vec_cds(sp_cds *A, double *x, double *y)
{
	int i, j, k, diag, ini, fin;

	/* Inicializa vector y */
	for (j = 0; j < A->nrow; ++j)
		y[j] = 0;

	for (k=0; k < A->ndiag; ++k) {		/* Por cada diagonal no vacía en A */
		diag = A->dx[k];

		/* Calcula inicio y fin de la diagonal en la fila de val */
		ini = MAX(0, -diag);
		fin = A->maxdiaglength - MAX(0, diag);

		for (i=ini; i < (A->maxdiaglength); ++i) {	/* Por cada elemento de la diagonal */
			j = diag + i;
			y[i] += ( A->val[k][i] * x[j] );
		}
	}
}

/** \fn sp_cds* mult_mat_cds(sp_cds *A, sp_cds *B);
 *  \brief Multiplica dos matrices en formato CDS.
 *
 *  \param A Matriz operando izquierdo de la multiplicación
 *  \param B Matriz operando derecho de la multiplicación
 *
 *  Esta operación computa la operación \f$C = AB\f$ y devuelve un puntero a la matriz \c C.
 *
 *  La implementación de esta operación está dividida en dos etapas claramente diferenciadas:
 *  <OL>
 *   <LI>Cálculo anticipado de la estructura de \c C (cuyo objetivo es equivalente al de una factorización simbólica)</LI>
 *   <LI>Cálculo numérico de \c C</LI>
 *  </OL>
 *
 *  La lógica de ambas etapas está fundamentada en la misma observación que puede hacerse del algoritmo <em>"clásico"</em> de multiplicación
 *  de matrices. Recordando nuestra definición de diagonales (el elemento \f$a_{ij}\f$ pertenece a la diagonal \f$j-i\f$) y observando
 *  el algoritmo clásico
 *  \f[
 *  c_{ij} = \sum_{k=0}^n a_{ik}b_{kj}
 *  \f]
 *  obtenemos las siguientes conclusiones:
 *
 *  Primero, toda diagonal \c t de \c C se verá afectada únicamente por productos de entradas de todo par de diagonales \c r y \c s de \c A
 *  y \c B respectivamente, tales que <TT>t = r+s</TT>. Para convencernos de esto, basta ver el algoritmo clásico. Vemos que el
 *  elemento \f$c_{ij}\f$ (perteneciente a la diagonal <TT>(j-i)</TT>) se ve afectado por la multiplicación de \f$a_{ik}b_{kj}\f$
 *  para todo \c k. Es decir, un elemento de la diagonal <TT>(k-i)</TT> de \c A y un elemento de la diagonal <TT>(j-k)</TT> de \c B.
 *  Por lo tanto, como tenemos que <TT>(k-i)+(j-k) = j-i</TT>, nuestro enunciado es correcto.
 *
 *  Esta observación nos da una pauta de cómo realizar ambas etapas del algoritmo. En particular, para la etapa de cálculo de la estructura
 *  del resultado \c C, podemos afirmar lo siguiente: Si la diagonales \c r de \c A y \c s de \c B son no vacías (entendiendo por diagonal
 *  vacía aquella con cero en todas sus entradas), entonces la diagonal <TT>(r+s)</TT> de \c C (si existe) es no vacía.
 *
 *  La acotación "si existe" del enunciado anterior es porque van a haber diagonales que no se van a cruzar en el producto. Basta ver
 *  cómo el algoritmo calcula las diagonales máxima y mínima (variables \c mindiag y \c maxdiag) antes de comenzar con el cálculo de
 *  la estructura.
 *
 *  Con esto tenemos resuelto el cálculo de la estructura de \c C. Observamos que también será una matriz de banda, pero más diagonales
 *  tendrán valores no cero.
 *
 *  Con respecto al cálculo de las entradas de \c C, hace falta aclarar cómo se multiplican las entradas de las diagonales de \c A y \c B.
 *  Ya vimos que un elemento de la diagonal de \c A se multiplicará con uno de \c B. Pero, ¿cuál con cuál?
 *
 *  Observamos, gracias al algoritmo "clásico", que todo producto de entradas de \c A y \c B siempre cumple que la columna de \c A
 *  se corresponde con la misma fila de \c B (ambas tienen el mismo índice \c k en el algoritmo). Por lo tanto, dadas dos diagonales \c r
 *  y \c s de \c A y \c B respectivamente, basta encontrar el primer par de elementos que cumplan con esta condición, para que los siguientes
 *  pares (avanzando un lugar en cada una de las diagonales) también lo cumpla. Esto es lo que hace el algoritmo en la etapa comentada
 *  como la "alineación" de las diagonales a multiplicar. Luego de esto, se hace el producto entrada por entrada.
 *
 *  \note
 *  La etapa de cálculo de la estructura ejecuta en un tiempo que está en el orden <em>O(A.ndiag+B.ndiag)</em>,
 *  mientras que el cálculo numérico ejecuta en un tiempo que está en el orden <em>O(D(A)+D(B))</em>, siendo <TT>D(X)</TT> la cantidad
 *  de entradas (con valor cero o no cero) que componen todas las diagonales no vacías de \c X.
 */
sp_cds* mult_mat_cds(sp_cds *A, sp_cds *B)
{
	unsigned int n, m, tope, dxl, ka, kb, i, j, k;
	int mindiag, maxdiag, diaga, diagb, diag, inia, fina, inib, finb, ja, diff;
	sp_cds* c;

	if (A->ncol != B->nrow) {
		BAL_ERROR("Las dimensiones de las matrices no permiten su multiplicación");
		return NULL;
	}

	c = (sp_cds*)malloc(sizeof(sp_cds));
	c->nrow = n = A->nrow;
	c->ncol = m = B->ncol;
	c->maxdiaglength = MIN(n, m);
	tope = m + n - 1; /* Cantidad máxima de diagonales */
	c->dx = (int*)malloc(sizeof(int) * tope);

	/* Calcula las diagonales que tendrán datos en el resultado */
	mindiag = 1 - n;
	maxdiag = m - 1;
	dxl = 0;
	for (ka=0; ka < A->ndiag; ++ka) {		/* Por cada diagonal con datos en A */
		diaga = A->dx[ka];
		for (kb=0; kb < B->ndiag; ++kb) {	/* Por cada diagonal con datos en B */
			diagb = B->dx[kb];

			diag = diaga + diagb;			/* Diagonal en C que tendrá datos */
			if (diag >= mindiag && diag <= maxdiag) {
				if (binary_search(c->dx, dxl, diag) == -1) {
					insert_sorted(c->dx, dxl - 1, diag);
					++dxl;
				}
			}
		}
	}

	/* Inicializa c->val */
	c->ndiag = dxl;
	c->val = (double**)malloc(sizeof(double*) * dxl);
	for (i=0; i < dxl; ++i) {
		c->val[i] = (double*)malloc(sizeof(double) * c->maxdiaglength);
		for (j=0; j < c->maxdiaglength; ++j)
			c->val[i][j] = 0;
	}

	/* Calcula los valores de c */
	for (ka=0; ka < A->ndiag; ++ka) {		/* Por cada diagonal con datos en A */
		diaga = A->dx[ka];
		for (kb=0; kb < B->ndiag; ++kb) {	/* Por cada diagonal con datos en B */
			diagb = B->dx[kb];

			diag = diaga + diagb;
			k = binary_search(c->dx, dxl, diag);

			inia = MAX(0, -diaga);
			fina = A->maxdiaglength - MAX(0, diaga);
			inib = MAX(0, -diagb);
			finb = B->maxdiaglength - MAX(0, diagb);

			/* Alinea las diagonales para multiplicar */
			ja = A->dx[ka] + inia;
			diff = ja - inib;
			if (diff > 0)
				inib += diff;
			else
				inia -= diff;

			for (i=inia,j=inib; i < fina && j < finb; ++i, ++j)
				c->val[k][i] += ( A->val[ka][i] * B->val[kb][j] );
		}
	}

	return c;
}

