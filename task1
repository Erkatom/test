#include <iostream>
#include <new>

int ** convert(const int * t, size_t n, const size_t * lns, size_t rows)
{
  int ** mtx = new int * [rows];
  size_t allocated_rows = 0;

  try
  {

    for (size_t i = 0; i < rows; ++i)
    {
      
      mtx[i] = new int [row_len];

      for (size_t j = 0; j < row_len; ++j)
      {
        mtx[i][j] = t[current_t_idx++];
      }
    }
  }
  catch (const std::bad_alloc & e)
  {
    for (size_t i = 0; i < allocated_rows; ++i)
    {
      delete [] mtx[i];
    }
    delete [] mtx;
    
  }

  return mtx;
}
