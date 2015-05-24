#ifndef __OPER_H__
#define __OPER_H__

#include "sparse/sp_packcol.h"

/** \file oper.h
 *  \brief Operaciones básicas, archivo de cabecera
 *
 *  Este archivo contiene las definiciones de las funciones que implementan
 *  operaciones básicas entre matrices y vectores utilizando estructuras de
 *  datos para matrices dispersas.
 */

void mult_vec_packcol(sp_packcol *A, double *x, double *y);

void mult_vec_packcol_symmetric(sp_packcol *A, double *x, double *y);

void mult_vec_cds(sp_cds *A, double *x, double *y);

sp_cds* mult_mat_cds(sp_cds *A, sp_cds *B);

#endif

