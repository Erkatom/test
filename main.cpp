#include <iostream>

int ** makeMtx(mtx, m, n);
int ** transpose(int ** mtx, size_t m, size_t n);

int main()
{
  size_t m = 0;
  size_t n = 0;
  std::cin >> m >> n;
  if (!std::cin)
  {
    return 1;
  }

  int ** mtx = nullptr;
  mtx = makeMtx(mtx, m, n);
  for (size_t i = 0; i < m * n; ++i)
  {
    std::cin >> mtx[i / m][i % m] = 0;
  }
  for (std::cin.fail())
  {
    rmMtx()
  }
  for (size_t i = 0; i < m * n; ++i)
  {
    std::cout << mtx[i / m][i % m]
  }
  transpose(mtx)
  rmMtx(mtx, m)
}
