#include <stdio.h>
#include <stdlib.h>
#include <bal.h>

void mult_vec_packcol_test()
{
	double **matriz = NULL;
	sp_coord* sp = NULL;
	sp_packcol* sp1 = NULL;
	int i, n, m;
	double x[] = { 1.0, 2.1, 3.2, 4.3, 5.4, 6.5 };
	double *y;

	printf(" -- mult_vec_packcol_test -- \n");

	if (bal_cargar_matriz("./data/matriz_sim_6x6.m", &matriz, &n, &m) == 0) {
		sp = bal_mat2coord(n, m, matriz);
		sp1 = bal_coord2packcol(sp);

		printf("EMPAQUETADO POR COLUMNA:\n");
		bal_imprimir_packcol(stdout, sp1);

		y = (double*)malloc(sizeof(double) * sp1->nrow);

		bal_mult_vec_packcol(sp1, x, y);

		printf("Resultado de la multiplicacion:\n");
		for(i=0; i < sp1->nrow; ++i)
			printf("%f ", y[i]);
		printf("\n");

		bal_free_coord(sp);
		bal_free_packcol(sp1);
	}
	else
		printf("mul_vec_packcol_test: Error al leer el archivo\n");
}

int main()
{
	mult_vec_packcol_test();

	return 0;
}

