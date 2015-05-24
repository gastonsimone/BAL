#include <stdio.h>
#include <stdlib.h>
#include <bal.h>

void load_coord_test()
{
	sp_coord* sp = NULL;
	FILE *fp;

	printf(" -- load_coord_test -- \n");

	if ((fp = fopen("./data/matriz.coord", "r")) != NULL)
		sp = bal_load_coord(fp);

	if (sp != NULL) {
		printf("LOAD COORD:\n");
		bal_imprimir_coord(stdout, sp);
		bal_free_coord(sp);
	}
	else
		printf("load_coord_test: Error al leer el archivo\n");
}

int main()
{
	load_coord_test();

	return 0;
}

