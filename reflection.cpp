#include "reflection.h"
// установить размер и выделить память для отраж диаг блоков
execution_status
reflection::di_set_size_and_allocate_mem (size_t m)
{
  v_size = m;
  return memory_allocate ((m + 2) * (m - 1) / 2);
}
// установить размер и выделить память для отраж обыч блоков
execution_status
reflection::set_size_and_allocate_mem (size_t m)
{
  printf ("reflection::set_size_and_allocate_mem: m_size = %ld\n", m);
  v_size = m + 1;
  return memory_allocate ((m + 1) * m);
}
//
execution_status
reflection::diagonalization_of_block (block &bl)
{
  if (!bl.square ())
    {
      printf ("reflection::diagonalization_of_block: block is not square\n");
      return execution_status::bad_arg;
    }
  
  double *arr = get_arr (), *b_arr = bl.get_arr ();
  size_t index = 0, b_size = bl.get_col_num ();

  for (size_t i = 0; i < b_size - 1; i++)
    {
      double sum = 0;
      index++;
      size_t vec_size = b_size - i;
      
      // Вычисление нормы столбца
      for (size_t j = i + 1; j < b_size; j++)
        {
          size_t t = b_size * j + i;
          sum += b_arr[t] * b_arr[t];
          arr[index++] = b_arr[t];
          b_arr[t] = 0;
        }
      
      size_t t = i * b_size + i;
      index -= vec_size;
      arr[index] = b_arr[t];
      double sub_sum = sum;
      sum += b_arr[t] * b_arr[t];
      
      // ИСПРАВЛЕНИЕ: используем fabs вместо abs
      sum = sqrt (fabs (sum));
      b_arr[t] = sum;
      arr[index] -= b_arr[t];
      sub_sum += arr[index] * arr[index];
      sub_sum = sqrt (fabs (sub_sum));
      
      // ИСПРАВЛЕНИЕ: правильная проверка на ноль
      if (fabs(sub_sum) < EPSILON)
        for (size_t j = 0; j < vec_size; j++)
          arr[index + j] = 0;
      else
        for (size_t j = 0; j < vec_size; j++)
          arr[index + j] /= sub_sum;

      // Блочная обработка (размер блока = 5)
      size_t k = (b_size - i - 1) / 5;
      size_t b_stop = i + 1 + k * 5;

      // Обработка по блокам 5x5
      for (size_t j = i + 1; j < b_stop; j += 5)
        {
          double s[5] = {0, 0, 0, 0, 0};
          size_t stop = vec_size - vec_size % 5;
          
          // Вычисление скалярных произведений
          for (size_t u = 0; u < stop; u += 5)
            {
              double *row0 = b_arr + b_size * (i + u) + j;
              double *row1 = row0 + b_size;
              double *row2 = row1 + b_size;
              double *row3 = row2 + b_size;
              double *row4 = row3 + b_size;
              double *x_row = arr + index + u;

              for (int col = 0; col < 5; col++) {
                s[col] += row0[col] * x_row[0] + 
                         row1[col] * x_row[1] +
                         row2[col] * x_row[2] + 
                         row3[col] * x_row[3] +
                         row4[col] * x_row[4];
              }
            }
          
          // Остаточная обработка
          for (size_t u = stop; u < vec_size; u++)
            {
              double *row = b_arr + b_size * (i + u) + j;
              double x_val = arr[index + u];
              for (int col = 0; col < 5; col++)
                s[col] += row[col] * x_val;
            }
          
          // Применение преобразования
          for (size_t u = 0; u < stop; u += 5)
            {
              double *row0 = b_arr + b_size * (i + u) + j;
              double *row1 = row0 + b_size;
              double *row2 = row1 + b_size;
              double *row3 = row2 + b_size;
              double *row4 = row3 + b_size;
              double *x_row = arr + index + u;

              for (int col = 0; col < 5; col++) {
                row0[col] -= 2 * x_row[0] * s[col];
                row1[col] -= 2 * x_row[1] * s[col];
                row2[col] -= 2 * x_row[2] * s[col];
                row3[col] -= 2 * x_row[3] * s[col];
                row4[col] -= 2 * x_row[4] * s[col];
              }
            }
          
          for (size_t u = stop; u < vec_size; u++)
            {
              double *row = b_arr + b_size * (i + u) + j;
              double x_val = arr[index + u];
              for (int col = 0; col < 5; col++)
                row[col] -= 2 * x_val * s[col];
            }
        }
      
      // Обработка оставшихся столбцов
      for (size_t j = b_stop; j < b_size; j++)
        {
          double sum = 0;
          for (size_t u = 0; u < vec_size; u++)
            sum += b_arr[b_size * (i + u) + j] * arr[index + u];
          for (size_t u = 0; u < vec_size; u++)
            b_arr[b_size * (i + u) + j] -= 2 * arr[index + u] * sum;
        }
      
      index += vec_size;
    }
  
  return execution_status::success;
}

execution_status
reflection::spread_diagonalization (block &b)
{
  double *bl_arr = b.get_arr ();
  size_t r_n = b.get_row_num ();
  size_t c_n = b.get_col_num ();
  double *arr = get_arr ();
  
  size_t arr_index = 0;
  size_t reflections_count = r_n - 1; // Для квадратной матрицы исходного блока
  
  for (size_t i = 0; i < reflections_count; i++)
    {
      size_t vec_size = r_n - i;  // Размер вектора отражения
      
      // Для каждого столбца правого блока
      for (size_t col = 0; col < c_n; col++)
        {
          double sum = 0;
          
          // Вычисляем скалярное произведение
          for (size_t j = 0; j < vec_size; j++)
            {
              size_t row = i + j;
              sum += bl_arr[c_n * row + col] * arr[arr_index + j];
            }
          
          // Применяем преобразование
          for (size_t j = 0; j < vec_size; j++)
            {
              size_t row = i + j;
              bl_arr[c_n * row + col] -= 2 * arr[arr_index + j] * sum;
            }
        }
      
      arr_index += vec_size;  // Переходим к следующему вектору
    }
  
  return execution_status::success;
}
// обнуление поддиагональных блоков
execution_status
reflection::block_reset (block &bl, block &di_bl)
{
  if (!di_bl.square ())
    {
      printf ("reflection::block_reset: diagonal block is not square\n");
      return execution_status::bad_arg;
    }
  
  size_t r_n = bl.get_row_num ();      // строк в поддиагональном блоке
  size_t c_n = bl.get_col_num ();      // столбцов в поддиагональном блоке
  size_t di_b_size = di_bl.get_row_num (); // размер диагонального блока
  
  if (c_n != di_b_size)
    {
      printf ("reflection::block_reset: column count mismatch\n");
      return execution_status::bad_arg;
    }
  
  if (get_m_size () < (r_n + 1) * c_n)
    {
      printf ("reflection::block_reset: small size\n");
      return execution_status::runtime_error;
    }
  
  v_size = r_n + 1;
  double *b_arr = bl.get_arr ();
  double *di_b_arr = di_bl.get_arr ();
  double *arr = get_arr ();
  size_t arr_in = 0;
  
  for (size_t i = 0; i < c_n; i++)
    {
      double sum = 0;
      
      // Сохраняем элементы поддиагонального блока и обнуляем их
      for (size_t j = 0; j < r_n; j++)
        {
          double val = b_arr[j * c_n + i];
          sum += val * val;
          arr[arr_in + j + 1] = val;  // Сохраняем в arr[1..r_n]
          b_arr[j * c_n + i] = 0;     // Обнуляем поддиагональный блок
        }
      
      // Вычисляем норму расширенного вектора [diagonal_element; subdiagonal_block_column]
      double diag_val = di_b_arr[i * di_b_size + i];
      double squared_norm = sum + diag_val * diag_val;
      double norm = sqrt(fabs(squared_norm));
      
      // Строим вектор отражения v
      arr[arr_in] = diag_val - norm;  // arr[0] = diagonal_element - norm
      di_b_arr[i * di_b_size + i] = norm;  // Обновляем диагональный элемент
      
      // Нормализуем вектор отражения
      double v_norm_sq = sum + arr[arr_in] * arr[arr_in];
      double v_norm = sqrt(fabs(v_norm_sq));
      
      if (fabs(v_norm) < EPSILON)
        {
          for (size_t u = 0; u < r_n + 1; u++)
            arr[arr_in + u] = 0;
        }
      else
        {
          for (size_t u = 0; u < r_n + 1; u++)
            arr[arr_in + u] /= v_norm;
        }
      
      // Применяем отражение к оставшимся столбцам
      for (size_t j = i + 1; j < c_n; j++)
        {
          double dot_product = 0;
          
          // Вычисляем скалярное произведение: v^T * [diagonal_row; subdiagonal_column]
          dot_product += di_b_arr[i * di_b_size + j] * arr[arr_in];  // diagonal part
          
          for (size_t u = 0; u < r_n; u++)
            dot_product += b_arr[u * c_n + j] * arr[arr_in + u + 1];  // subdiagonal part
          
          // Обновляем элементы: x = x - 2 * dot_product * v
          di_b_arr[i * di_b_size + j] -= 2 * dot_product * arr[arr_in];
          
          for (size_t u = 0; u < r_n; u++)
            b_arr[u * c_n + j] -= 2 * dot_product * arr[arr_in + u + 1];
        }
      
      arr_in += r_n + 1;
    }
  
  return execution_status::success;
}
// распространение обнуления поддиагональных блоков на правый блок
execution_status
reflection::block_reset_spread (block &bl, block &di_bl)
{
  size_t r_n = bl.get_row_num ();      // строк в блоке
  size_t c_n = bl.get_col_num ();      // столбцов в блоке
  size_t di_b_size = di_bl.get_row_num (); // размер диагонального блока
  
  if (c_n != di_b_size)
    {
      printf ("reflection::block_reset_spread: size mismatch\n");
      return execution_status::bad_arg;
    }
  
  double *b_arr = bl.get_arr ();
  double *di_b_arr = di_bl.get_arr ();
  double *arr = get_arr ();
  
  size_t total_reflections = c_n;  // Количество построенных отражений
  size_t arr_index = 0;            // Текущая позиция в массиве векторов
  
  // Последовательно применяем каждое отражение ко всем столбцам блока
  for (size_t ref_idx = 0; ref_idx < total_reflections; ref_idx++)
    {
      size_t reflection_size = r_n + 1;  // Размер вектора отражения
      
      // Применяем отражение ко всем столбцам блока (начиная с ref_idx)
      for (size_t col = ref_idx; col < c_n; col++)
        {
          double dot_product = 0;
          
          // Вычисляем скалярное произведение: v^T * x
          // x состоит из: [диагональный элемент; столбец поддиагонального блока]
          
          // Диагональная часть
          dot_product += di_b_arr[di_b_size * ref_idx + col] * arr[arr_index];
          
          // Поддиагональная часть  
          for (size_t row = 0; row < r_n; row++)
            {
              dot_product += b_arr[row * c_n + col] * arr[arr_index + row + 1];
            }
          
          // Обновляем элементы: x = x - 2 * dot_product * v
          
          // Обновляем диагональный элемент
          di_b_arr[di_b_size * ref_idx + col] -= 2 * dot_product * arr[arr_index];
          
          // Обновляем поддиагональный блок
          for (size_t row = 0; row < r_n; row++)
            {
              b_arr[row * c_n + col] -= 2 * dot_product * arr[arr_index + row + 1];
            }
        }
      
      // Переходим к следующему вектору отражения
      arr_index += reflection_size;
    }
  
  return execution_status::success;
}
