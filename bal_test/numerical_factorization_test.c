#include <stdio.h>
#include <stdlib.h>
#include <bal.h>

void numerical_factorization_test()
{
	double **matriz = NULL;
	sp_coord* sp = NULL;
	sp_packcol* sp1 = NULL;
	sp_packcol* spL = NULL;
	int i, n, m;

	printf(" -- numerical_factorization_test -- \n");

	if (bal_cargar_matriz("./data/matriz_sim_6x6.m", &matriz, &n, &m) == 0) {
		sp = bal_mat2coord(n, m, matriz);
		sp1 = bal_coord2packcol_symmetric(sp);

		spL = bal_symbolic_factorization(sp1);
		bal_numerical_factorization(sp1, spL);
		printf("FACTORIZACION NUMERICA:\n");
		bal_imprimir_packcol(stdout, spL);

		bal_free_coord(sp);
		bal_free_packcol(sp1);
		bal_free_packcol(spL);
	}
	else
		printf("numerical_factorization_test: Error al leer el archivo\n");
}

int main()
{
	numerical_factorization_test();

	return 0;
}

