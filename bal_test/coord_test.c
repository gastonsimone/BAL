#include <stdio.h>
#include <stdlib.h>
#include <bal.h>

void coord_test()
{
	double **matriz = NULL;
	sp_coord* sp = NULL;
	int n, m;

	printf(" -- coord_test -- \n");

	if (bal_cargar_matriz("./data/matriz_sim_6x6.m", &matriz, &n, &m) == 0) {
		sp = bal_mat2coord(n, m, matriz);
		printf("POR COORDENADAS:\n");
		bal_imprimir_coord(stdout, sp);

		bal_free_coord(sp);
	}
	else
		printf("coord_test: Error al leer el archivo\n");
}

int main()
{
	coord_test();

	return 0;
}

