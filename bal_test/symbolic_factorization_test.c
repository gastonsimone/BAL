#include <stdio.h>
#include <stdlib.h>
#include <bal.h>

void symbolic_factorization_test()
{
	double **matriz = NULL;
	sp_coord* sp = NULL;
	sp_packcol* sp1 = NULL;
	sp_packcol* spL = NULL;
	int i, n, m;

	printf(" -- symbolic_factorization_test -- \n");

	if (bal_cargar_matriz("./data/matriz_sim_6x6.m", &matriz, &n, &m) == 0) {
		sp = bal_mat2coord(n, m, matriz);
		sp1 = bal_coord2packcol_symmetric(sp);
		printf("EMPAQUETADO POR COLUMNA (SIMETRICO):\n");
		bal_imprimir_packcol(stdout, sp1);

		spL = bal_symbolic_factorization(sp1);
		printf("FACTORIZACION SIMBOLICA:\n");
		bal_imprimir_packcol(stdout, spL);

		bal_free_coord(sp);
		bal_free_packcol(sp1);
		bal_free_packcol(spL);		
	}
	else
		printf("symbolic_factorization_test: Error al leer el archivo\n");
}

int main()
{
	symbolic_factorization_test();

	return 0;
}

