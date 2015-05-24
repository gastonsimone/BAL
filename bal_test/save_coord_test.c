#include <stdio.h>
#include <stdlib.h>
#include <bal.h>

void save_coord_test()
{
	double **matriz = NULL;
	sp_coord* sp = NULL;
	int i, n, m;

	printf(" -- save_coord_test -- \n");

	if (bal_cargar_matriz("./data/matriz_diag_5x5.m", &matriz, &n, &m) == 0) {
		sp = bal_mat2coord(n, m, matriz);
		printf("SAVE coord:\n");
		bal_save_coord(stdout, sp);

		bal_free_coord(sp);
	}
	else
		printf("save_coord_test: Error al leer el archivo\n");
}

int main()
{
	save_coord_test();

	return 0;
}

