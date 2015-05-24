#include <stdio.h>
#include <stdlib.h>
#include <bal.h>

void row_traversal_packcol_test()
{
	double **matriz = NULL;
	sp_coord* sp = NULL;
	sp_packcol* sp1 = NULL;
	int n, m;
	
	int x, i, j, posij;

	printf(" -- row_traversal_packcol_test -- \n");

	if (bal_cargar_matriz("./data/matriz_sim_6x6.m", &matriz, &n, &m) == 0) {
		sp = bal_mat2coord(n, m, matriz);
		sp1 = bal_coord2packcol_symmetric(sp);

		printf("RECORRER POR FILAS EMPAQUETADO POR COLUMNA (SIMETRICA):\n");
		
		i = -2;
		bal_row_traversal_packcol(sp1, &i, &j, &posij);	/* Inicializacion */

		for (x = 0; x < sp1->nrow; ++x) {									/* Por cada fila */
			while (bal_row_traversal_packcol(sp1, &i, &j, &posij) != -1) {	/* Mientras no lleguemos al final de la fila */
				printf("(%d) [%d,%d] = %f\n", x, i, j, sp1->val[posij]);
			}
			printf("Fin linea %d.\n", x);
		}

		/* Liberamos la memoria utilizada por este procedimiento */
		i = -2;
		bal_row_traversal_packcol(NULL, &i, &j, &posij);
		bal_free_coord(sp);
		bal_free_packcol(sp1);
	}
	else
		printf("row_traversal_packcol_test: Error al leer el archivo\n");
}

int main()
{
	row_traversal_packcol_test();

	return 0;
}

