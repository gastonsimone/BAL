#include <stdio.h>
#include <stdlib.h>
#include <bal.h>

void mult_mat_cds_test()
{
	double **matriz = NULL;
	sp_coord* spA = NULL;
	sp_cds* cdsA = NULL;
	sp_coord* spB = NULL;
	sp_cds* cdsB = NULL;
	sp_cds* cdsC = NULL;
	int i, n, m;

	printf(" -- mult_mat_cds_test -- \n");

	if (bal_cargar_matriz("./data/matriz_diag_5x5.m", &matriz, &n, &m) == 0) {
		spA = bal_mat2coord(n, m, matriz);
		cdsA = bal_coord2cds(spA);

		spB = bal_mat2coord(n, m, matriz);
		cdsB = bal_coord2cds(spB);

		cdsC = bal_mult_mat_cds(cdsA, cdsB);

		printf("Resultado de la multiplicacion:\n");
		bal_imprimir_cds(stdout, cdsC);

		bal_free_coord(spA);
		bal_free_coord(spB);
		bal_free_cds(cdsA);
		bal_free_cds(cdsB);
		bal_free_cds(cdsC);
	}
	else
		printf("mul_mat_cds_test: Error al leer el archivo\n");
}

int main()
{
	mult_mat_cds_test();

	return 0;
}

