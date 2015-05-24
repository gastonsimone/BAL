#include <stdio.h>
#include <stdlib.h>
#include <bal.h>

void print_array(double *a, int n)
{
	int i;

	for (i = 0; i < n; ++i)
		printf("%f ", a[i]);
	printf("\n");
}

void cholesky_test()
{
	double **matriz = NULL;
	sp_coord* sp = NULL;
	sp_packcol* sp1 = NULL;
	int i, n, m;
	double b[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};

	printf(" -- cholesky_test -- \n");

	if (bal_cargar_matriz("./data/matriz_sim_6x6.m", &matriz, &n, &m) == 0) {
		sp = bal_mat2coord(n, m, matriz);
		sp1 = bal_coord2packcol_symmetric(sp);
		printf("MATRIZ:\n");
		bal_imprimir_packcol(stdout, sp1);

		printf("ARRAY b:\n");
		print_array(b, 6);

		bal_cholesky_solver(sp1, b);
		printf("RESULTADO DE Ax=b:\n");
		print_array(b, 6);

		bal_free_coord(sp);
		bal_free_packcol(sp1);
	}
	else
		printf("cholesky_test: Error al leer el archivo\n");
}

int main()
{
	cholesky_test();

	return 0;
}

