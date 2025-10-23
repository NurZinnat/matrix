#ifndef REFLECTION_H
#define REFLECTION_H

#include "block.h"

class reflection : public data
{
  size_t v_size{};

public:
  reflection () = default;
  ~reflection () = default;
  execution_status di_set_size_and_allocate_mem (size_t m);
  execution_status set_size_and_allocate_mem (size_t m);
  execution_status diagonalization_of_block (block &bl);
  size_t get_vector_x_from_one_block (size_t Index, size_t block_size,
                                             double *bl, size_t x_v_index,
                                             double *x_v);
  execution_status spread_diagonalization (block &b);
  execution_status block_reset (block &bl, block &di_bl);
  execution_status block_reset_spread (block &bl, block &di_bl);
};

#endif // REFLECTION_H
