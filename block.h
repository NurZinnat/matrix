#ifndef BLOCK_H
#define BLOCK_H

#include "sub_matrix.h"

#define EPSILON 1e-16

class block : public sub_matrix
{
public:
  block () = default;
  ~block () = default;
  execution_status inverse_upper_triangular (block &x);
  execution_status mult (block &x, block &y);
  execution_status sum_mult (block &x, block &y);

  inline execution_status sub_multiply_matrix (block &x, block &y);
  inline execution_status sum_sub_multiply_matrix (block &x, block &y);
};

#include "multy.inc"

#include "sum_multy.inc"

#endif // BLOCK_H