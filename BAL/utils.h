#ifndef __BAL_UTILS__
#define __BAL_UTILS__

#include <stdio.h>
#include "utils.h"

/** \file utils.h
 *  \brief Archivo con definiciones y funciones de utilidad general
 */

/** \def BAL_ERROR(x)
 *  \brief Función para reportar errores detectados por código de BAL.
 *
 *  Esta macro es de uso interno de BAL y no está disponible desde código externo.
 */
#define BAL_ERROR(x) fprintf(stderr, "bal:%s:%d: %s\n", __FILE__, __LINE__, (x))

int binary_search(int *list, unsigned int size, int key);

void insert_sorted(int *list, int n, int key);

#endif

