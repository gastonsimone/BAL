#include <stdio.h>
#include <stdlib.h>
#include <bal.h>

void elimination_tree_test()
{
	double **matriz = NULL;
	sp_coord* sp = NULL;
	sp_packcol* sp1 = NULL;
	int i, n, m, nnz;
	int *parents;

	printf(" -- elimination_tree_test -- \n");

	if (bal_cargar_matriz("./data/matriz_sim_6x6.m", &matriz, &n, &m) == 0) {
		sp = bal_mat2coord(n, m, matriz);
		sp1 = bal_coord2packcol_symmetric(sp);
		printf("EMPAQUETADO POR COLUMNA (SIMETRICO):\n");
		bal_imprimir_packcol(stdout, sp1);

		parents = bal_elimination_tree(sp1, &nnz);
		printf("Cantidad de elementos en la factorizacion de CHolesky: %d\n", nnz);
		printf("ARBOL DE ELIMINACION:\n");

		for (i = 0; i < sp1->nrow; ++i)
			printf("[%d] = %d\n", i, parents[i]);

		bal_free_coord(sp);
		bal_free_packcol(sp1);
		free(parents);
	}
	else
		printf("elimination_tree_test: Error al leer el archivo\n");
}

int main()
{
	elimination_tree_test();

	return 0;
}

