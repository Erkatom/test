#include <iostream>

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

    for (size_t i = 0; i < m * n; ++i)[cite: 2]
    {
        std::cin >> mtx[i % m][i / m];[cite: 2]
    }

    if (std::cin.fail())[cite: 2]
    {
        rmMtx(mtx, m);[cite: 2]
        return 1;[cite: 2]
    }

    transpose(mtx);[cite: 1, 2]

    if (m > 0 && n > 0)
    {
        std::cout << mtx[0][0];[cite: 1, 2]
        for (size_t i = 1; i < m; ++i)[cite: 1, 2]
        {
            std::cout << ' ' << mtx[0][i];[cite: 1, 2]
        }

        for (size_t i = 1; i < n; ++i)[cite: 1, 2]
        {
            std::cout << '\n' << mtx[i][0];[cite: 1, 2]
            for (size_t j = 1; j < m; ++j)[cite: 1, 2]
            {
                std::cout << ' ' << mtx[i][j];[cite: 1, 2]
            }
        }
    }

    std::cout << '\n';
    rmMtx(mtx, m);
    return 0;
}
