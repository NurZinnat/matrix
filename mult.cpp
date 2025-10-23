#include "matrix_inverse.h"

int
main (int argc, char *argv[])
{
  if (argc < 3)
    {
      printf ("usegw\n");
      return 0;
    }
  block a, c;
  int row = 0, col = 0;
  // /int mid = 0;
  sscanf (argv[1], "%d", &row);
  //sscanf (argv[2], "%d", &mid);
  sscanf (argv[2], "%d", &col);

  a.init (row, col, matrix_type::upper_triangular, 0, argv[3]);
  printf ("входная матрица\n");
  a.print (row, col);
  //b.init (mid, col, matrix_type::ordinary, 0, argv[5]);
  c.memory_allocate (row * col);
  c.set_col_num (row);
  c.set_row_num (col);
  c.zero_padding ();

  c.inverse_upper_triangular (a);
  
  // double t1 = clock ();
  // c.mult (a, b);
  // t1 = (clock () - t1) / CLOCKS_PER_SEC;
  

  // c.print (row, row);

  // printf ("\n\nnew\n\n");

  // double t2 = clock ();
  // c.sub_multiply_matrix (a, b);
  // t2 = (clock () - t2) / CLOCKS_PER_SEC;

  printf ("обратная матрица\n");
  
  c.print (row, row);

  //printf ("\n\nt1 = %lf t2 = %lf\n\n\n", t1, t2);

  return 0;
}