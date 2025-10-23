#include "sub_matrix.h"

// get_row_num
size_t
sub_matrix::get_row_num () const
{
  return row_num;
}
// get_col_num
size_t
sub_matrix::get_col_num () const
{
  return col_num;
}
// get_m_type
matrix_type
sub_matrix::get_m_type () const
{
  return m_type;
}
// moov_constructor
sub_matrix::sub_matrix (sub_matrix &&x) : data ((data &&)x)
{
  row_num = x.row_num;
  col_num = x.col_num;
  m_type = x.m_type;
  x.row_num = 0;
  x.col_num = 0;
  x.m_type = matrix_type::empty;
}
// moov_operator
sub_matrix &
sub_matrix::operator= (sub_matrix &&x)
{
  if (&x == this)
    return *this;
  data::operator= ((data &&)x);
  row_num = x.row_num;
  col_num = x.col_num;
  m_type = x.m_type;
  x.m_type = matrix_type::empty;
  return *this;
}
// инициализция через файл или формулу
execution_status
sub_matrix::init (size_t row_n, size_t col_n, matrix_type _m_type, int s,
                  bool flag, char *file_name)
{
  if (flag)
    {
      execution_status alloc_status = memory_allocate (row_n * col_n);
      if (alloc_status != execution_status::success)
        return alloc_status;
      row_num = row_n;
      col_num = col_n;
      // printf ("m_size = %ld\n", get_m_size ());
      if (_m_type == matrix_type::empty)
        {
          printf ("sub_matrix::init: allocate memory for empty matix\n");
          return execution_status::bad_arg;
        }
    }
  m_type = _m_type;
  int n = row_n;
  switch (s)
    {
    case 0:
      {
        if (!file_name)
          {
            printf ("sub_matrix::init: invalid file_name\n");
            return execution_status::bad_arg;
          }
        FILE *fp = fopen (file_name, "r");
        if (!fp)
          {
            printf ("sub_matrix::init: can not open file %s\n", file_name);
            return execution_status::open_file;
          }
        for (size_t i = 0; i < get_m_size (); i++)
          {
            if (fscanf (fp, "%lf", &get_arr ()[i]) != 1)
              {
                printf ("sub_matrix::init: can not read double from file %s\n",
                        file_name);
                fclose (fp);
                return execution_status::read_file;
              }
          }
        fclose (fp);
        break;
      }
    case 1:
      {
        for (size_t i = 0; i < row_num; i++)
          for (size_t j = 0; j < col_num; j++)
            get_arr ()[i * col_num + j] = n - std::max (i + 1, j + 1);
        break;
      }
    case 2:
      {
        for (size_t i = 0; i < row_num; i++)
          for (size_t j = 0; j < col_num; j++)
            get_arr ()[i * col_num + j] = std::max (i + 1, j + 1);
        break;
      }
    case 3:
      {
        for (size_t i = 0; i < row_num; i++)
          for (size_t j = 0; j < col_num; j++)
            get_arr ()[i * col_num + j]
                = n - abs (double (i + 1) - double (j + 1));
        break;
      }
    case 4:
      {
        for (size_t i = 0; i < row_num; i++)
          for (size_t j = 0; j < col_num; j++)
            get_arr ()[i * col_num + j] = 1 / double (i + 1 + j + 1) - 1;
        break;
      }
    default:
      break;
    }
  return execution_status::success;
}

void
sub_matrix::print (size_t _row_num, size_t _col_num)
{
  size_t r = std::min (row_num, _row_num);
  size_t c = std::min (col_num, _col_num);
  for (size_t i = 0; i < r; i++)
    {
      for (size_t j = 0; j < c; j++)
        printf (" %10.10lf", get_arr ()[i * col_num + j]);
      printf ("\n");
    }
  printf ("\n");
}
// set_m_type
void
sub_matrix::set_m_type (matrix_type _m_type)
{
  m_type = _m_type;
}
// set_row_col
void
sub_matrix::set_row_num (size_t _row_num)
{
  row_num = _row_num;
}
// set_col_num
void
sub_matrix::set_col_num (size_t _col_num)
{
  col_num = _col_num;
}
// провекра на пустоту
bool
sub_matrix::empty ()
{
  if (get_m_size () == 0)
    return true;
  return false;
}
// обнуление
execution_status
sub_matrix::zero_padding ()
{
  if (empty ())
    {
      printf ("попытка обнулить пустую матрицу\n");
      return execution_status::runtime_error;
    }
  size_t stop = row_num * col_num;
  double *arr = get_arr ();
  for (size_t i = 0; i < stop; i++)
    arr[i] = 0;
  set_m_type (matrix_type::zero);
  return execution_status::success;
}
// присвоение  единичной квадратной матрицы
execution_status
sub_matrix::identity_padding ()
{
  if (empty ())
    {
      printf ("попытка присвоить  единичную матрицу пустой матрице\n");
      return execution_status::runtime_error;
    }
  if (row_num != col_num)
    {
      printf ("попытка присвоить  единичную матрицу пустой матрице\n");
      return execution_status::runtime_error;
    }
  zero_padding ();
  double *arr = get_arr ();
  for (size_t i = 0; i < row_num; i++)
    arr[col_num * i + i] = 1;
  set_m_type (matrix_type::identity);
  return execution_status::success;
}
// operator += for block
sub_matrix &
sub_matrix::operator+= (const sub_matrix &x)
{
  size_t _row_num = get_row_num (), _col_num = get_col_num ();
  size_t x_row_num = x.get_row_num (), x_col_num = x.get_col_num ();
  double *arr = get_arr (), *x_arr = x.get_arr ();
  if (_row_num != x_row_num)
    {
      printf ("sub_matrix::operator+= не совпадают кол-во строк\n");
      return *this;
    }
  else if (_col_num != x_col_num)
    {
      printf ("sub_matrix::operator+= не совпадают кол-во стоблцоы\n");
      return *this;
    }
  size_t stop = _row_num * _col_num;
  for (size_t i = 0; i < stop; i++)
    arr[i] += x_arr[i];
  return *this;
}
// operator -= for block
sub_matrix &
sub_matrix::operator-= (const sub_matrix &x)
{
  size_t _row_num = get_row_num (), _col_num = get_col_num ();
  size_t x_row_num = x.get_row_num (), x_col_num = x.get_col_num ();
  double *arr = get_arr (), *x_arr = x.get_arr ();
  if (_row_num != x_row_num)
    {
      printf ("sub_matrix::operator+= не совпадают кол-во строк\n");
      return *this;
    }
  else if (_col_num != x_col_num)
    {
      printf ("sub_matrix::operator+= не совпадают кол-во стоблцоы\n");
      return *this;
    }
  size_t stop = _row_num * _col_num;
  for (size_t i = 0; i < stop; i++)
    arr[i] -= x_arr[i];
  return *this;
}
// квадратная ли ?
bool
sub_matrix::square ()
{
  if (empty ())
    return false;
  if (row_num == col_num)
    return true;
  return false;
}

void
sub_matrix::init_row_col (size_t _row_num, size_t _col_num)
{
  set_row_num (_row_num);
  set_col_num (_col_num);
}