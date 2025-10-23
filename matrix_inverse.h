#ifndef MATRIX_INVERSE_H
#define MATRIX_INVERSE_H

#include "matrix.h"

class matrix_inverse
{
  matrix *A{};
  matrix *B{};

  block x{};
  block y{};
  block z{};

  reflection di_ref{};
  reflection ref{};

  size_t size{};
  size_t b_size{};
  size_t k{};

  double *res{};

public:
  matrix_inverse ();
  ~matrix_inverse ();
  execution_status set_info (matrix &_A, matrix &_B, int _bl_size);
  execution_status diagonalization ();
  execution_status Gaussian_inverse ();
  double discrepancy (bool flag);
};

#endif // MATRIX_INVERSE_H