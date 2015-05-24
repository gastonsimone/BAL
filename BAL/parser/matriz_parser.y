/** \file matriz_parser.y
 *  \brief Archivo de definición del parser generado por bison.
 * 
 * Este archivo es la entrada de bison, mediante el cual se genera el código C que implementa el parser
 * que lee definiciones de matrices en formato matlab, produciendo el archivo <TT>matriz_parser.tab.c</TT>.
 *
 * El parser está definido de forma que cargue en memoria la matriz parseada en formato de matrices convencional del lenguaje C
 * mediante el puntero <TT>double ***matriz</TT>.
 */

/** \fn void yyerror(const char *archivo, double ***matriz, int *n, int *m, const char *msg)
 *	\brief Función de manejo de errores para el parser generado por bison
 *
 *	\param archivo Camino al archivo donde esta definida la matriz
 *	\param matriz Puntero en donde se va a devolver la matriz parseada (en caso de recuperarse del error)
 *  \param n Cantidad de filas de la matriz leída
 *  \param m Cantidad de columnas de la matriz leída
 *  \param msg Mensaje de error devuelto por el parser generado por bison
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <errno.h>
#include <string.h>

/** \fn int yylex (void)
 *	\brief Analizador lexicográfico generado por flex, invocado por el parser generado por bison para solicitar un nuevo token.
 */
int yylex (void);

void yyerror(const char *archivo, double ***matriz, int *n, int *m, const char *msg);

/** \fn void yyrestart (FILE *fp)
 *	\brief Función definida por el analizador lexicográfico generado por flex para establecer el archivo desde el cual se leerá.
 *
 *	\param fp Puntero al archivo que se leerá.
 */
void yyrestart (FILE *fp);
%}

%union {
	char car;
	double valor;
	GSList *lista;
}

%parse-param {const char *archivo};
%parse-param {double ***matriz};
%parse-param {int *n};
%parse-param {int *m};
%initial-action{
	FILE* fp;

	if (NULL == (fp = fopen(archivo, "r"))) {
		fprintf(stderr, "[%d] %s\n", errno, strerror(errno));
		YYABORT;
	}

	yyrestart(fp);
};

%token <car> APERTURA CERRADO SEMICOLON
%token <valor> NUMERO		/* Una entrada de la matriz*/
%type <lista> matriz	/* La matriz como lista de filas */
%type <lista> filas		/* Lista de filas */
%type <lista> fila		/* Una fila como lista de valores */
%destructor { g_slist_free($$); } matriz filas fila

%%

matriz : APERTURA filas CERRADO	
	{
		GSList *aux, *fila;
		double *val;
		int i, j;

		$$ = $2;

		*matriz = NULL;
		*n = (int)g_slist_length($2);
		aux = (GSList*)$2->data;
		*m = (int)g_slist_length(aux);

		/* Antes verificar que todas las filas tienen la misma cantidad de valores */
		for(aux=$2->next; aux; aux=aux->next) {
			fila = (GSList*)aux->data;

			if (*m != (int)g_slist_length(fila))
				YYABORT;
		}

		/* Armar estructura en memoria */
		*matriz = (double**)malloc(sizeof(double*) * (*n));
		for(aux=$2, i=0; aux; aux=aux->next, ++i) {
			(*matriz)[i] = (double*)malloc(sizeof(double) * (*m));
			for(fila = (GSList*)aux->data, j=0; fila; fila=fila->next, ++j) {
				val = (double*)fila->data;
				(*matriz)[i][j] = *val;
				free(fila->data);
			}
		}
	}
	;

filas : fila				{ $$ = g_slist_append(NULL, (gpointer)$1); }
	| filas SEMICOLON fila	{ $$ = g_slist_append($1, (gpointer)$3); }
	;

fila : NUMERO	
	 {
		double *val = (double *)malloc(sizeof(double));
		*val = $1;
	 	$$ = g_slist_append(NULL, (gpointer)val);
	}
	| fila NUMERO	
	{
		double *val = (double *)malloc(sizeof(double));
		*val = $2;
		$$ = g_slist_append($1, (gpointer)val);
	}
	;

%%

void yyerror(const char *archivo, double ***matriz, int *n, int *m, const char *msg)
{
	fprintf(stderr, "%s\n", msg);
}

