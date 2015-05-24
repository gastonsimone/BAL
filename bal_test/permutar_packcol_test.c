#include <stdio.h>
#include <stdlib.h>
#include <bal.h>

void permutar_packcol_test()
{
	double **matriz = NULL;
	sp_coord* sp = NULL;
	sp_packcol* sp1 = NULL;
	sp_packcol* sp2 = NULL;
	int n, m;
	unsigned int p[] = {0, 3, 2, 6, 5, 4, 1};

	printf(" -- permutar_packcol_test -- \n");

	if (bal_cargar_matriz("./data/matriz_7x7.m", &matriz, &n, &m) == 0) {
		sp = bal_mat2coord(n, m, matriz);

		sp1 = bal_coord2packcol(sp);
		printf("EMPAQUETADO POR COLUMNA (COMPLETO):\n");
		bal_save_packcol(stdout, sp1);

		printf("PERMUTACION:\n");
		for (n=0; n < 7; ++n) {
			printf(" %d", p[n]);
		}
		printf("\n");

		sp2 = bal_permutar_packcol(p, sp1);
		printf("PERMUTACION:\n");
		bal_save_packcol(stdout, sp2);

		bal_free_coord(sp);
		bal_free_packcol(sp1);
		bal_free_packcol(sp2);		
	}
	else
		printf("permutar_packcol_test: Error al leer el archivo\n");
}

int main()
{
	permutar_packcol_test();

	return 0;
}

