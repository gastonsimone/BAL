#include <stdio.h>
#include <stdlib.h>
#include <bal.h>

void packcol_test()
{
	double **matriz = NULL;
	sp_coord* sp = NULL;
	sp_packcol* sp1 = NULL;
	int n, m;

	printf(" -- packcol_test -- \n");

	if (bal_cargar_matriz("./data/matriz_sim_6x6.m", &matriz, &n, &m) == 0) {
		sp = bal_mat2coord(n, m, matriz);
		sp1 = bal_coord2packcol(sp);
		printf("EMPAQUETADO POR COLUMNA (COMPLETO):\n");
		bal_imprimir_packcol(stdout, sp1);

		bal_free_coord(sp);
		bal_free_packcol(sp1);
	}
	else
		printf("packcol_test: Error al leer el archivo\n");
}

int main()
{
	packcol_test();

	return 0;
}

