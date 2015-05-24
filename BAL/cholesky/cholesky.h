#ifndef __CHOLESKY_H__
#define __CHOLESKY_H__

#include "../sparse/sp_packcol.h"

/** \file cholesky.h
 *  \brief Archivo de cabecera para la factorización de Cholesky.
 *
 *  Este archivo define las funciones que implementan la factorización
 *  de Cholesky para matrices dispersas. Por más detalles acerca de la
 *  implementación, vea la documentación del archivo cholesky.c
 */

int *elimination_tree(sp_packcol *A, int *nnz);

sp_packcol *symbolic_factorization(sp_packcol *A);

void numerical_factorization(sp_packcol *A, sp_packcol *L);

void cholesky_Lsolver(sp_packcol *L, double *b);

void cholesky_LTsolver(sp_packcol *L, double *b);

void cholesky_solver(sp_packcol *A, double *b);

#endif

