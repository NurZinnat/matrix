#include "matrix_inverse.h"

int
main (int argc, char *argv[])
{
  int n = 0, m = 0, r = 0, s = 0;
  char *file_name = nullptr;
  if (argc < 5)
    {
      printf ("usage\n");
      return 0;
    }
  if (sscanf (argv[1], "%d", &n) != 1)
    {
      printf ("incorrect format of cmd parametr n\n");
      return 0;
    }
  if (sscanf (argv[2], "%d", &m) != 1)
    {
      printf ("incorrect format of cmd parametr n\n");
      return 0;
    }
  if (sscanf (argv[3], "%d", &r) != 1)
    {
      printf ("incorrect format of cmd parametr n\n");
      return 0;
    }
  if (sscanf (argv[4], "%d", &s) != 1)
    {
      printf ("incorrect format of cmd parametr n\n");
      return 0;
    }
  if (s == 0)
    {
      if (argc != 6)
        {
          printf ("не задано имя файла\n");
          return 0;
        }
      file_name = argv[5];
    }
  if (n < 0 || m < 0 || r < 0 || s < 0)
    {
      printf ("incorrect cmd arg\n");
      return 0;
    }
  if (n == 0 || m == 0)
    {
      printf ("incorrect cmd arg\n");
      return 0;
    }
  //_________________________________________________
  matrix A, B;
  try
    {
      A.init (size_t (n), size_t (n), matrix_type::ordinary, size_t (s), true,
              file_name);
      B.memory_allocate (size_t (n) * size_t (n));
      B.init_row_col (size_t (n), size_t (n));
      B.identity_padding ();
      // B.print (10, 10);
    }
  catch (const std::exception &e)
    {
      std::cerr << e.what () << '\n';
      return 0;
    }
  matrix_inverse MatrixInverse;
  try
    {
      MatrixInverse.set_info (A, B, size_t (m));
    }
  catch (const std::exception &e)
    {
      printf ("%s", e.what ());
      return 0;
    }
  //__________________________________________________
  if (m > n)
    m = n;
  // printf ("Входная матрица\n\n");
  // A.print (10, 10);

  // printf ("FLAG_1_B\n\n\n");
  // B.print (10, 10);

  double t1 = clock ();
  MatrixInverse.diagonalization ();

  // printf ("diagonalization\n\n\n");

  // printf ("A:\n");

  // A.print (3, 3);

  // printf ("B:\n");

  // B.print (3, 3);

  printf ("_______________________________________\n");


  MatrixInverse.Gaussian_inverse ();
  

  t1 = (clock () - t1) / CLOCKS_PER_SEC;
   printf ("Обратная матрица\n\n");
   B.print (10, 10);
  double r1 = 0, r2 = 0, t2 = 0;

  A.init (size_t (n), size_t (n), matrix_type::ordinary, size_t (s), false,
              file_name);

  t2 = clock ();
  r1 = MatrixInverse.discrepancy (true);
  r2 = MatrixInverse.discrepancy (false);
  t2 = (clock () - t2) / CLOCKS_PER_SEC;

  int task = 24;
  printf ("%s : Task = %d Res1 = %e Res2 = %e T1 = %.2f T2 = %.2f S = %d N = "
          "%d M = %d\n",
          argv[0], task, r1, r2, t1, t2, s, n, m);

  return 0;
}