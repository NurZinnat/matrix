#include "matrix.h"
// get_block
execution_status
matrix::get_block (block &bl, size_t size, size_t r_index, size_t c_index)
{
  // if (size > 32)
  //   printf ("_______________________________________matrix::diagonal_spread size = %ld\n", size);
  // if (r_index > 32 || c_index > 32)
  //   {
  //     printf ("SUKA\n");
  //   }
  size_t b_ind = 0;
  size_t r_n = get_row_num () - r_index * size;
  size_t c_n = get_col_num () - c_index * size;
  size_t col_num = get_col_num ();
  size_t row_num = get_row_num ();
  double *bl_arr = bl.get_arr ();
  double *arr = get_arr ();
  r_index = r_index * size;
  c_index = c_index * size;
  r_n = row_num - r_index >= size ? size : row_num % size;
  c_n = col_num - c_index >= size ? size : col_num % size;
  bl.set_row_num (r_n);
  bl.set_col_num (c_n);
  for (size_t i = r_index; i < r_index + r_n; i++)
    for (size_t j = c_index; j < c_index + c_n; j++)
      bl_arr[b_ind++] = arr[col_num * i + j];
  set_m_type (matrix_type::ordinary);
  return execution_status::success;
}
// put_block
execution_status
matrix::put_block (block &bl, size_t size, size_t r_index, size_t c_index)
{
  // if (size > 32)
  //   printf ("_______________________________________matrix::diagonal_spread size = %ld\n", size);
  size_t b_ind = 0;
  size_t col_num = get_col_num ();
  size_t row_num = get_row_num ();
  size_t r_n = row_num - r_index * size;
  size_t c_n = col_num - c_index * size;
  double *bl_arr = bl.get_arr ();
  double *arr = get_arr ();
  r_index = r_index * size;
  c_index = c_index * size;
  r_n = row_num - r_index >= size ? size : row_num % size;
  c_n = col_num - c_index >= size ? size : col_num % size;
  for (size_t i = r_index; i < r_index + r_n; i++)
    for (size_t j = c_index; j < c_index + c_n; j++)
      arr[col_num * i + j] = bl_arr[b_ind++];
  bl.set_m_type (matrix_type::ordinary);
  return execution_status::success;
}
// put_block_minus
execution_status
matrix::put_block_minus (block &bl, size_t size, size_t r_index,
                         size_t c_index)
{
  // if (size > 32)
  //   printf ("_______________________________________matrix::diagonal_spread size = %ld\n", size);
  size_t b_ind = 0;
  size_t col_num = get_col_num ();
  size_t row_num = get_row_num ();
  size_t r_n = row_num - r_index * size;
  size_t c_n = col_num - c_index * size;
  double *bl_arr = bl.get_arr ();
  double *arr = get_arr ();
  r_index = r_index * size;
  c_index = c_index * size;
  r_n = row_num - r_index >= size ? size : row_num % size;
  c_n = col_num - c_index >= size ? size : col_num % size;
  for (size_t i = r_index; i < r_index + r_n; i++)
    for (size_t j = c_index; j < c_index + c_n; j++)
      arr[col_num * i + j] -= bl_arr[b_ind++];
  bl.set_m_type (matrix_type::ordinary);
  return execution_status::success;
}
// распространение отражения диаг блоков на всю матрицу
execution_status
matrix::diagonal_spread (block &bl, size_t size, reflection &ref,
                         size_t r_index, size_t c_in_start, size_t c_in_finish)
{
  // if (size > 32)
  //   printf ("matrix::diagonal_spread size = %ld\n", size);
  size_t col_num = get_col_num ();
  size_t k = col_num / size;
  if (col_num % size)
    k++;
  for (size_t i = c_in_start; i < c_in_finish; i++)
    {
      get_block (bl, size, r_index, i);
      // bl.print (10, 10);
      ref.spread_diagonalization (bl);
      // bl.print (10, 10);
      put_block (bl, size, r_index, i);
    }
  return execution_status::success;
}
// распространение обнуления поддиагональных блоков на всю матрицу
execution_status
matrix::other_spread (block &bl, size_t size, block &di_bl, reflection &ref,
                      size_t r_index, size_t di_r_index, size_t k)
{
  // size_t row_num = get_row_num ();
  // size_t col_num = get_col_num ();
  for (size_t i = di_r_index + 1; i < k; i++)
    {
      // printf ("matrix::other_spread i = %ld size = %ld di_r_index = %ld\n",
      // i,
      //       size, di_r_index);
      get_block (di_bl, size, di_r_index, i);
      get_block (bl, size, r_index, i);
      //printf ("   flag\n");
      ref.block_reset_spread (bl, di_bl);
      put_block (di_bl, size, di_r_index, i);
      put_block (bl, size, r_index, i);
    }
  return execution_status::success;
}
// распространение обнуления поддиагональных блоков на всю матрицу
execution_status
matrix::other_spread_B (block &bl, size_t size, block &di_bl, reflection &ref,
                        size_t r_index, size_t di_r_index)
{
  // size_t row_num = get_row_num ();
  // size_t col_num = get_col_num ();
  for (size_t i = 0; i <= di_r_index; i++)
    {
      // printf ("matrix::other_spread i = %ld size = %ld di_r_index = %ld\n",
      // i, size, di_r_index);
      get_block (di_bl, size, di_r_index, i);
      get_block (bl, size, r_index, i);
      // printf ("   flag\n");
      ref.block_reset_spread (bl, di_bl);
      put_block (di_bl, size, di_r_index, i);
      put_block (bl, size, r_index, i);
    }
  get_block (di_bl, size, di_r_index, r_index);
  get_block (bl, size, r_index, r_index);
  // printf ("   flag\n");
  ref.block_reset_spread (bl, di_bl);
  put_block (di_bl, size, di_r_index, r_index);
  put_block (bl, size, r_index, r_index);
  return execution_status::success;
}
// распространение обратного метода гаусса на присоединенную матрицу
execution_status
matrix::gaussian_block_inverse(size_t block_index, size_t block_size, 
                              matrix &A, block &temp1, block &temp2, block &temp3)
{
  size_t total_blocks = (get_col_num() + block_size - 1) / block_size;
  
  // 1. Обращаем диагональный блок A[block_index,block_index]
  A.get_block(temp1, block_size, block_index, block_index);
  temp1.set_m_type(matrix_type::upper_triangular);
  
  execution_status status = temp2.inverse_upper_triangular(temp1);
  if (status != execution_status::success)
    return status;
  
  // 2. Умножаем строку блоков расширенной матрицы на обратную диагональную
  for (size_t j = 0; j < total_blocks; j++)
    {
      get_block(temp1, block_size, block_index, j);      // B[block_index,j]
      //temp3.sub_multiply_matrix(temp2, temp1);
      temp3.mult (temp2, temp1);         // temp3 = A⁻¹[block_index,block_index] * B[block_index,j]
      put_block(temp3, block_size, block_index, j);      // B[block_index,j] = temp3
    }
  
  // 3. Обнуляем столбец над текущим блоком в расширенной матрице
  for (int i = block_index - 1; i >= 0; i--)
    {
      // Для всех блоков выше: B[i,j] = B[i,j] - A[i,block_index] * B[block_index,j]
      A.get_block(temp2, block_size, i, block_index);    // temp2 = A[i,block_index]
      
      for (size_t j = 0; j < total_blocks; j++)
        {
          get_block(temp1, block_size, block_index, j);  // temp1 = B[block_index,j]
          
          // Вычисляем произведение: temp3 = A[i,block_index] * B[block_index,j]
          temp3.sub_multiply_matrix(temp2, temp1);
          
          // Вычитаем из текущего блока расширенной матрицы
          put_block_minus(temp3, block_size, i, j);      // B[i,j] -= temp3
        }
    }
  
  return execution_status::success;
}

