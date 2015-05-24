#include <stdio.h>
#include <stdlib.h>
#include <bal.h>

void save_cds_test()
{
	double **matriz = NULL;
	sp_coord* sp = NULL;
	sp_cds* sp1 = NULL;
	int i, n, m;

	printf(" -- save_cds_test -- \n");

	if (bal_cargar_matriz("./data/matriz_diag_5x5.m", &matriz, &n, &m) == 0) {
		sp = bal_mat2coord(n, m, matriz);
		sp1 = bal_coord2cds(sp);
		printf("SAVE CDS:\n");
		bal_save_cds(stdout, sp1);

		bal_free_coord(sp);
		bal_free_cds(sp1);
	}
	else
		printf("save_cds_test: Error al leer el archivo\n");
}

int main()
{
	save_cds_test();

	return 0;
}

