#include <stdio.h>
#include <stdlib.h>
#include <bal.h>

void parser_test()
{
	double **matriz = NULL;
	int n, m;

	printf(" -- parser_test -- \n");

	if (bal_cargar_matriz("./data/matriz_sim_6x6.m", &matriz, &n, &m) == 0) {
		printf("MATRIZ:\n");
		bal_imprimir_matriz(stdout, matriz, n, m);
	}
	else
		printf("parser_test: Error al leer el archivo\n");
}

int main()
{
	parser_test();

	return 0;
}

