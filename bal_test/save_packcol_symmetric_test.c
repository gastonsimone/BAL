#include <stdio.h>
#include <stdlib.h>
#include <bal.h>

void save_packcol_symmetric_test()
{
	double **matriz = NULL;
	sp_coord* sp = NULL;
	sp_packcol* sp1 = NULL;
	int i, n, m;

	printf(" -- save_packcol_symmetric_test -- \n");

	if (bal_cargar_matriz("./data/matriz_sim_6x6.m", &matriz, &n, &m) == 0) {
		sp = bal_mat2coord(n, m, matriz);
		sp1 = bal_coord2packcol_symmetric(sp);
		printf("SAVE PACKCOL:\n");
		bal_save_packcol_symmetric(stdout, sp1);

		bal_free_coord(sp);
		bal_free_packcol(sp1);
	}
	else
		printf("save_packcol_symmetric_test: Error al leer el archivo\n");
}

int main()
{
	save_packcol_symmetric_test();

	return 0;
}

