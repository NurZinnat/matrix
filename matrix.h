#ifndef MATRIX_H
#define MATRIX_H

#include "reflection.h"

class matrix : public sub_matrix
{
public:
  matrix () = default;
  ~matrix () = default;

  execution_status get_block (block &bl, size_t size, size_t r_index,
                              size_t c_index);
  execution_status put_block (block &bl, size_t size, size_t r_index,
                              size_t c_index);
  execution_status put_block_minus (block &bl, size_t size, size_t r_index,
                                    size_t c_index);
  execution_status diagonal_spread (block &bl, size_t size, reflection &ref,
                                    size_t row_index, size_t c_in_start,
                                    size_t c_in_finish);
  execution_status other_spread (block &bl, size_t size, block &di_bl,
                                 reflection &ref, size_t r_index,
                                 size_t di_r_index, size_t k);
  execution_status other_spread_B (block &bl, size_t size, block &di_bl,
                                 reflection &ref, size_t r_index,
                                 size_t di_r_index);

  execution_status multiply_by_inverse (matrix &A, size_t size, block &x,
                                        block &y, block &z, size_t index);
  execution_status gaussian_block_inverse(size_t block_index, size_t block_size, 
                              matrix &A, block &temp1, block &temp2, block &temp3);
};

#endif // MATRIX_H