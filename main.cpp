#include <iostream>

// haha

int makeMtx(int mtx, size_t m, size_t n);
void transpose(int** mtx);
void rmMtx(int** mtx, size_t m);

int main()
{
    size_t m = 0;
    size_t n = 0;
    std::cin >> m >> n;
    if (!std::cin)
    {
        return 1;
    }

    int** mtx = nullptr;
    mtx = makeMtx(mtx, m, n);

    for (size_t i = 0; i < m * n; ++i)
    {
        std::cin >> mtx[i % m][i / m];
    }

    if (std::cin.fail())
    {
        rmMtx(mtx, m);
        return 1;
    }

    transpose(mtx);

    if (m > 0 && n > 0)
    {
        std::cout << mtx[0][0];
        for (size_t i = 1; i < m; ++i)
        {
            std::cout << ' ' << mtx[0][i];
        }

        for (size_t i = 1; i < n; ++i)
        {
            std::cout << '\n' << mtx[i][0];
            for (size_t j = 1; j < m; ++j)
            {
                std::cout << ' ' << mtx[i][j];
            }
        }
    }

    std::cout << '\n';
    rmMtx(mtx, m);
    return 0;
}
