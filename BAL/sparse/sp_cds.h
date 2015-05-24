#ifndef __SP_CDS_H__
#define __SP_CDS_H__

#include <stdio.h>
#include "sp_coord.h"

/** \file sp_cds.h
 *	\brief Archivo de cabecera para matriz dispersa, formato CDS (comprimido por diagonal).
 *
 *	Este archivo contiene la definición de la estructura de datos
 *	mediante la cual se almacena una matriz dispersa según el formato
 *	comprimido por diagonal, conocido como CDS (por sus siglas en inglés
 *	<em>Compressed Diagonal Storage</em>).
 */

/** \struct sp_cds
 *  \brief Estructura de matriz dispersa CDS
 *
 *  Almacena una matriz dispersa en memoria utilizando el formato CDS.
 *
 *  Los valores se almacenan de la siguiente forma.
 *  A cada diagonal se le asigna un índice que la identifica. La diagonal mayor es la diagonal 0.
 *  Las diagonales superiores a la mayor tienen índices positivos que se van incrementando a medida que nos
 *  alejamos de la diagonal mayor.
 *  Las diagonales inferiores a la mayor tienen índices negativos que se van decrementando a medida que nos alejamos
 *  de la diagonal mayor.
 *  Por ejemplo, las primeras subdiagonales superior e inferior les corresponden los índices 1 y -1 respectivamente.
 *
 *  De este modo, el elemento \f$A_{ij}\f$ de la matriz está ubicado en la diagonal \f$j-i\f$.
 *
 *  Solo se almacenan las diagonales con al menos un valor no cero.
 *  Cada fila en \c val es una diagonal de \c A. El arreglo \c dx indica a qué diagonal corresponde
 *  cada una de las filas de \c val. Es decir, si queremos obtener valores
 *  de la diagonal \c i, éstos están ubicados en la fila \c j de \c val tal que <TT>i = dx[j]</TT>.
 *
 *  Por otra parte, la ubicación de los valores de la diagonal en una fila de \c val está dada por el índice de fila
 *  en la matriz. Es decir \f$A_{ij}\f$ corresponde a la diagonal \f$j-i\f$ y su valor está en la columna \c i de \c val.
 *  Esto provoca que las diagonales inferiores estén alineadas a la derecha y las superiores alineadas a la izquierda.
 *
 *  Por ejemplo, sea la matriz
 *  \f[ A = 
 *  \left(
 *  \begin{array}{ccccc}
 *  1 & 2 & 0 &  0 &  0 \\
 *  3 & 4 & 5 &  0 &  0 \\
 *  0 & 6 & 7 &  8 &  0 \\
 *  0 & 0 & 9 & 10 & 11 \\
 *  0 & 0 & 0 & 12 & 13
 *  \end{array}\right)
 *  \f]
 *  La estructura CDS para \c A es la siguiente. La diagonal de menor índice es la -1 y la de mayor índice es la +1.
 *
 *  <CODE>
 *  dx = [-1 0 1]<BR>
 *  val[0,0:4] = [0 3 6  9 12]<BR>
 *  val[1,0:4] = [1 4 7 10 13]<BR>
 *  val[2,0:4] = [2 5 8 11  0]<BR>
 *  </CODE>
 *
 *  Resumiendo, podemos decir que el valor \f$A_{ij}\f$ se encuentra en <TT>val[k,i]</TT>,
 *  con <TT>(dx[k] = j - i)</TT>.
 *  Recíprocamente podemos decir que el elemento en <TT>val[k,i]</TT> corresponde al elemento \f$A_{i,(dx[k]+i)}\f$.
 *
 *  \note
 *  Observar que esta estructura guarda toda diagonal con al menos un elemento no cero de forma completa. Es decir,
 *  los ceros en una diagonal "no vacía" son guardados.
 */
typedef struct {
	unsigned int nrow;	/**< \brief Cantidad de filas */
	unsigned int ncol;	/**< \brief Cantidad de columnas */
	unsigned int ndiag;	/**< \brief Cantidad de diagonales con al menos una entrada no cero */
	unsigned int maxdiaglength; /**< \brief El largo de la diagonal más larga */
	int *dx;			/**< \brief Arreglo de índices de diagonal */
	double **val;		/**< \brief Matriz de valores. Cada fila es una diagonal. */
} sp_cds;

sp_cds* coord2cds(sp_coord *mat);

void sp_imprimir_cds(FILE *fp, sp_cds *mat);

void save_cds(FILE *fp, sp_cds *A);

void free_cds(sp_cds *A);

#endif

