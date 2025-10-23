#ifndef SUB_MATRIX_H
#define SUB_MATRIX_H

#include "data.h"

class sub_matrix : public data
{
private:
  size_t row_num{};
  size_t col_num{};
  matrix_type m_type{};

  // methods

public:
  sub_matrix () = default;
  sub_matrix (const sub_matrix &x) = default;
  sub_matrix &operator= (const sub_matrix &x) = default;
  sub_matrix (sub_matrix &&x);
  sub_matrix &operator= (sub_matrix &&x);
  execution_status init (size_t row_n, size_t col_n, matrix_type _m_type,
                         int s, bool flag, char *file_name = nullptr);
  sub_matrix &operator+= (const sub_matrix &x);
  sub_matrix &operator-= (const sub_matrix &x);

  void print (size_t _row_col, size_t _col_num);
  bool empty ();
  execution_status zero_padding ();
  execution_status identity_padding ();

  size_t get_row_num () const;
  size_t get_col_num () const;
  matrix_type get_m_type () const;

  void set_m_type (matrix_type _m_type);
  void set_row_num (size_t _row_num);
  void set_col_num (size_t _col_num);
  bool square ();
  void init_row_col (size_t _r_num, size_t _c_num);
};

#endif // SUB_MATRIX_H