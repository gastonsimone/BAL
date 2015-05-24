/** \file matriz_scanner.lex
 *  \brief Archivo de definición del analizador lexicográfico generado por flex
 * 
 * Este archivo es la entrada de la herramienta \c flex, mediante la cual se genera el código
 * C que implementa el analizador lexicográfico que reconoce los tokens necesarios para parsear una
 * matriz en formato matlab.
 */

%top{
#include <glib.h>
#include "matriz_parser.tab.h"
}

DIGITO	[0-9]

%%

[ \t\n]+	/* Ignorar espacio en blanco */
"["			return APERTURA;
"]"			return CERRADO;
";"			return SEMICOLON;
{DIGITO}+	yylval.valor = atof(yytext); return NUMERO;
{DIGITO}+"."{DIGITO}*	yylval.valor = atof(yytext); return NUMERO;
"."{DIGITO}+	yylval.valor = atof(yytext); return NUMERO;

"-"{DIGITO}+	yylval.valor = atof(yytext); return NUMERO;
"-"{DIGITO}+"."{DIGITO}*	yylval.valor = atof(yytext); return NUMERO;

%%

