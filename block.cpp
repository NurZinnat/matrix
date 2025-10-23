#include "block.h"
execution_status
block::mult (block &f, block &s)
{
  if (f.get_col_num () != s.get_row_num ())
    {
      printf ("не совпдают рамерности\n");
      return execution_status::bad_arg;
    }
  size_t r_n = f.get_row_num ();
  size_t c_n = s.get_col_num ();
  size_t mid_size = f.get_col_num ();
  size_t s_row_len = s.get_col_num ();
  size_t f_row_len = f.get_col_num ();
  double *f_arr = f.get_arr (), *s_arr = s.get_arr (), *arr = get_arr ();

  for (size_t i = 0; i < r_n; i++)
    for (size_t j = 0; j < c_n; j++)
      {
        double sum = 0;
        for (size_t k = 0; k < mid_size; k++)
          sum += f_arr[f_row_len * i + k] * s_arr[s_row_len * k + j];
        arr[c_n * i + j] = sum;
      }
  set_row_num (r_n);
  set_col_num (c_n);
  return execution_status::success;
}
execution_status
block::sum_mult (block &f, block &s)
{
  if (f.get_col_num () != s.get_row_num ())
    {
      printf ("не совпдают рамерности\n");
      return execution_status::bad_arg;
    }
  size_t r_n = f.get_row_num ();
  size_t c_n = s.get_col_num ();
  size_t mid_size = f.get_col_num ();
  size_t s_row_len = s.get_col_num ();
  size_t f_row_len = f.get_col_num ();
  double *f_arr = f.get_arr (), *s_arr = s.get_arr (), *arr = get_arr ();

  for (size_t i = 0; i < r_n; i++)
    for (size_t j = 0; j < c_n; j++)
      {
        double sum = 0;
        for (size_t k = 0; k < mid_size; k++)
          sum += f_arr[f_row_len * i + k] * s_arr[s_row_len * k + j];
        arr[c_n * i + j] = +sum;
      }
  set_row_num (r_n);
  set_col_num (c_n);
  return execution_status::success;
}
// sub сумма произведения блоков
// обращение треугольной матрицы. Обращается аргумент
execution_status
block::inverse_upper_triangular (block &x)
{
  //printf ("inverse_upper_triangular\n");
  //x.print (3, 3);
  //  return execution_status::success;
  if (&x == this)
    {
      printf ("block::inverse_upper_triangular:: исходный и результирующий "
              "блоки совпадают\n");
      return execution_status::bad_arg;
    }
  if (get_m_size () < x.get_m_size ())
    {
      printf ("blocl::inverse_upper_triangular:: результирующий блок меньше "
              "по размеру. Требуется перевыдление памяти\n");
    }
  if (x.get_m_type () != matrix_type::upper_triangular)
    {
      printf ("block::inverse_upper_triangular: обращаемая матрица не "
              "верхнетреугольная\n");
      return execution_status::bad_arg;
    }
  set_row_num (x.get_row_num ());
  set_col_num (x.get_col_num ());
  size_t r_n = get_row_num (), c_n = get_col_num ();
  double *arr = get_arr (), *x_arr = x.get_arr ();
  
  // Инициализируем всю матрицу нулями
  for (size_t i = 0; i < r_n; i++)
    for (size_t j = 0; j < c_n; j++)
      arr[c_n * i + j] = 0;
  
  // Обработка последней строки
  double divider = x_arr[c_n * (r_n - 1) + c_n - 1];
  if (fabs (divider) < EPSILON)
    {
      printf ("block::inverse_upper_triangular: small divider_suka\n");
      return execution_status::small_divider;
    }
  arr[c_n * (r_n - 1) + c_n - 1] = 1.0 / divider;
  
  // Обработка остальных строк снизу вверх
  for (int i = r_n - 2; i >= 0; i--)
    {
      divider = x_arr[c_n * i + i];
      if (fabs (divider) < EPSILON)
        {
          printf ("divider = %lf\n", divider);
          printf ("block::inverse_upper_triangular: small divider\n");
          return execution_status::small_divider;
        }
      
      // Диагональный элемент
      arr[c_n * i + i] = 1.0 / divider;
      
      // Наддиагональные элементы
      for (size_t j = i + 1; j < c_n; j++)
        {
          double sum = 0;
          for (size_t u = i + 1; u <= j; u++)
            sum += x_arr[c_n * i + u] * arr[c_n * u + j];
          arr[c_n * i + j] = -sum / divider;
        }
    }
  
  set_m_type (matrix_type::upper_triangular);
  //print (3, 3);
  // printf ("inverse_upper_triangular_END\n");
  return execution_status::success;
}
