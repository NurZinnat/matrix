#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cmath>

#include "execution_status.h"
#include "matrix_type.h"

class data
{
private:
  double *arr{};
  size_t m_size{};


public:
  virtual ~data ();
  data ();
  data (const data &x);
  data &operator= (const data &x);
  data (data &&x);
  data &operator= (data &&x);
  execution_status memory_allocate (size_t n);
  double *get_arr () const;
  size_t get_m_size () const;
  double *get_arr ();
  size_t get_m_size ();
};
#endif // DATA_H