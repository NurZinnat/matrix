#include "matrix_inverse.h"

matrix_inverse::matrix_inverse ()
    : A (nullptr), B (nullptr), x (), y (), z (), size (0), b_size (0), k (0),
      res (nullptr)
{
}
matrix_inverse::~matrix_inverse ()
{
  // printf ("~matrix_inverse : start\n");
  if (res)
    delete[] res;
  // printf ("~matrix_inverse : finish\n");
}
execution_status
matrix_inverse::set_info (matrix &_A, matrix &_B, int _bl_size)
{
  if (_A.empty ())
    throw std::invalid_argument ("matrix can not be empty\n");
  else if (_A.get_m_type () == matrix_type::zero)
    throw std::invalid_argument ("matrix can not be zero\n");
  if (!_A.square ())
    throw std::invalid_argument ("matrix can not be unsquare\n");
  size = _A.get_row_num ();
  if (_B.get_m_size () < size * size)
    {
      printf ("matrix_inversion::set_info: size B smaller then size A\n");
      return execution_status::bad_arg;
    }

  if (_B.get_m_type () != matrix_type::identity)
    {
      printf ("matrix_inversion::set_info: matrix B is not identity\n");
      return execution_status::bad_arg;
    }
  A = &_A;
  B = &_B;
  b_size = _bl_size;
  k = size / b_size;
  if (size % b_size)
    k++;
  if (x.get_m_size () < b_size * b_size)
    x.memory_allocate (b_size * b_size);
  if (y.get_m_size () < b_size * b_size)
    y.memory_allocate (b_size * b_size);
  if (z.get_m_size () < b_size * b_size)
    z.memory_allocate (b_size * b_size);
  x.set_col_num (b_size);
  x.set_row_num (b_size);

  y.set_col_num (b_size);
  y.set_row_num (b_size);

  z.set_col_num (b_size);
  z.set_row_num (b_size);

  x.zero_padding ();
  y.zero_padding ();
  z.zero_padding ();
  // printf ("TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT\n");
  // z.print (2, 2);
  res = new double[b_size];
  if (!res)
    throw std::runtime_error (
        "matrix_inverse::init: can not allocate memory\n");
  for (size_t i = 0; i < b_size; i++)
    res[i] = 0;

  execution_status status = ref.set_size_and_allocate_mem (b_size);

  if (status != execution_status::success)
    return status;
  di_ref.di_set_size_and_allocate_mem (size);
  if (status != execution_status::success)
    return status;
  return execution_status::success;
}
// диагонализация матрицы A
execution_status
matrix_inverse::diagonalization ()
{
  for (size_t i = 0; i < k; i++)
    {
      // printf ("matrix_inverse::diagonalization i = %ld\n", i);
      A->get_block (x, b_size, i, i);
      execution_status status = di_ref.diagonalization_of_block (x);
      A->put_block (x, b_size, i, i);
      // continue;(
      if (status != execution_status::small_divider)
        {
          A->diagonal_spread (x, b_size, di_ref, i, i + 1, k);
          B->diagonal_spread (y, b_size, di_ref, i, 0, i + 1);
        }
      // continue;
      //  B->print (40, 15);

      for (size_t j = i + 1; j < k; j++)
        {
          // printf ("   matrix_inverse::diagonalization j = %ld\n", j);
          A->get_block (x, b_size, j, i);
          A->get_block (y, b_size, i, i);
          ref.block_reset (x, y);
          A->put_block (x, b_size, j, i);
          A->put_block (y, b_size, i, i);
          A->other_spread (x, b_size, y, ref, j, i, k);
          // B->print (10, 10);
          // printf ("МЕСТО ПАДЕНИЯ \n");
          B->other_spread_B (x, b_size, y, ref, j, i);
          // printf ("FLAG\n");
        }
      // return execution_status::success;
    }
  return execution_status::success;
}

execution_status
matrix_inverse::Gaussian_inverse()
{
  size_t total_blocks = k;
  
  // Обработка блоков снизу вверх
  for (int i = total_blocks - 1; i >= 0; i--)
    {
      execution_status status = B->gaussian_block_inverse(
          i, b_size, *A, x, y, z);
      
      if (status != execution_status::success)
        return status;
    }
  
  return execution_status::success;
}
//вычисление невязки
double
matrix_inverse::discrepancy(bool flag)
{
    if (size > 11000)
        return 0;
    
    matrix *first = A, *second = B;
    if (flag)
    {
        first = B;
        second = A;
    }
    
    double max_res = 0;
    for (size_t u = 0; u < b_size; u++)
        res[u] = 0;
    
    for (size_t i = 0; i < k; i++)
    {
        for (size_t j = 0; j < k; j++)
        {
            z.zero_padding();
            
            // Вычисление произведения блоков: C[i,j] = Σ first[i,u] * second[u,j]
            for (size_t u = 0; u < k; u++)
            {
                first->get_block(x, b_size, i, u);  // ИСПРАВЛЕНИЕ: i, u вместо j, u
                second->get_block(y, b_size, u, j); // ИСПРАВЛЕНИЕ: u, j вместо u, i
                //z.sum_sub_multiply_matrix(x, y);
                z.mult (x, y);
            }
            
            size_t r_n = z.get_row_num(), c_n = z.get_col_num();
            double *arr = z.get_arr();
            
            if (i != j)
            {
                // Для недиагональных блоков: должны быть нулевыми
                for (size_t u = 0; u < c_n; u++)
                    for (size_t t = 0; t < r_n; t++)
                        res[u] += fabs(arr[t * c_n + u]);  // ИСПРАВЛЕНИЕ: fabs вместо abs
            }
            else
            {
                // Для диагональных блоков: должны быть единичными
                for (size_t u = 0; u < c_n; u++)
                {
                    for (size_t t = 0; t < u; t++)
                        res[u] += fabs(arr[t * c_n + u]);  // Нижний треугольник = 0
                    
                    res[u] += fabs(1.0 - arr[u * c_n + u]);  // Диагональ = 1
                    
                    for (size_t t = u + 1; t < r_n; t++)     // ИСПРАВЛЕНИЕ: t < r_n вместо t < u
                        res[u] += fabs(arr[t * c_n + u]);  // Верхний треугольник = 0
                }
            }
            
            // Обновляем максимальную невязку
            for (size_t u = 0; u < c_n; u++)
            {
                max_res = std::max(max_res, res[u]);
                res[u] = 0;
            }
        }
    }
    
    return max_res;
}