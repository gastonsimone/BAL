#include <stdio.h>
#include <stdlib.h>
#include <bal.h>

void cds_test()
{
	double **matriz = NULL;
	sp_coord* sp = NULL;
	sp_cds* sp1 = NULL;
	int n, m;

	printf(" -- cds_test -- \n");

	if (bal_cargar_matriz("./data/matriz_diag_5x5.m", &matriz, &n, &m) == 0) {
		printf("MATRIZ:\n");
		bal_imprimir_matriz(stdout, matriz, n, m);

		sp = bal_mat2coord(n, m, matriz);
		sp1 = bal_coord2cds(sp);
		printf("EMPAQUETADO POR DIAGONALES:\n");
		bal_imprimir_cds(stdout, sp1);

		bal_free_coord(sp);
		bal_free_cds(sp1);
	}
	else
		printf("cds_test: Error al leer el archivo\n");
}

int main()
{
	cds_test();

	return 0;
}

