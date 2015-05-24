#ifndef __REORDENAMIENTO_H__
#define __REORDENAMIENTO_H__

#include "../sparse/sp_packcol.h"

/** \file reordenamiento.h
 *	\brief Archivo de cabecera para algoritmos de reordenamiento
 *
 *  Este archivo contiene (o contendrá) implementaciones de algoritmos de reordenamiento.
 *
 *  El reordenamiento es el primer paso en la resolución eficiente de sistemas de ecuaciones representados
 *  mediante matrices dispersas. El reordenamiento previo de una matriz tiene como principal objetivo la
 *  minimización del <em>fill-in</em> producido por las etapas de factorización que lo suceden.
 */

unsigned int* reordenar_packcol(sp_packcol *A);

sp_packcol* permutar_packcol(unsigned int *p, sp_packcol *A);

#endif

