#include "data.h"
// destructor
data::~data ()
{
  //printf ("data::~data: start\n");
  if (arr)
    delete[] arr;
  //printf ("data::~data: finish\n");
}
// constructor
data::data () : arr (nullptr), m_size (0) {}
// copy_constructor
data::data (const data &x)
{
  if (!x.arr)
    {
      arr = nullptr;
      m_size = 0;
      return;
    }
  arr = new double[x.m_size];
  if (!arr)
    throw std::runtime_error ("data::data: can not allocate mamory\n");
  for (size_t i = 0; i < x.m_size; i++)
    arr[i] = x.arr[i];
}
// copy_operator
data &
data::operator= (const data &x)
{
  if (&x == this)
    return *this;
  if (!x.arr)
    {
      if (arr)
        delete[] arr;
      arr = nullptr;
      m_size = 0;
      return *this;
    }
  if (x.m_size < m_size)
    {
      for (size_t i = 0; i < x.m_size; i++)
        arr[i] = x.arr[i];
      m_size = x.m_size;
      return *this;
    }
  else
    throw std::runtime_error (
        "data::copy_operator: can not allocate memory\n");
  return *this;
}
// moov_contructor
data::data (data &&x)
{
  arr = x.arr;
  m_size = x.m_size;
  x.arr = nullptr;
  x.m_size = 0;
}
// moov_operator
data &
data::operator= (data &&x)
{
  if (&x == this)
    return *this;
  arr = x.arr;
  m_size = x.m_size;
  x.arr = nullptr;
  x.m_size = 0;
  return *this;
}
// get_arr
double *
data::get_arr () const
{
  return arr;
}
// get_size
size_t
data::get_m_size () const
{
  return m_size;
}
// get_arr
double *
data::get_arr ()
{
  return arr;
}
// get_size
size_t
data::get_m_size ()
{
  return m_size;
}
// memory_allocate
execution_status
data::memory_allocate (size_t n)
{
  if (arr)
    {
      printf ("data::memory_allocate: memory has already allocated\n");
      return execution_status::runtime_error;
    }
  if (n == 0)
    {
      printf ("memory allocate size is zero\n");
      return execution_status::bad_arg;
    }
  arr = new double[n];
  if (!arr)
    throw std::runtime_error (
        "data::allocate_memory: can not allocate memory\n");
  m_size = n;
  return execution_status::success;
}



