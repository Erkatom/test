#include <iostream>
int  makeMtx(int  mtx,size_t m,size_t n);
int  transpose(int  mtx);
void rmMtx(int  mtx, size_t m);
int main()
{
        size_t m = 0;
        size_t n = 0;
        std::cin >> m >> n;
        if (!std::cin)
        {
                return 1;
        }
        int  mtx = nullptr;
        mtx = makeMtx(mtx,m,n);
        for (size_t i=0;i<m*n;++i)
        {
                std::cin >> mtx[i/n][i%n];
        }
        if (std::cin.fail())
        {
                rmMtx(mtx,m);
                return 1;
        }
        transpose(mtx);
        for (size_t i=0;i<m*n;++i) 
        { 
                std::cout << mtx[i/m][i%m];
        }
        rmMtx(mtx,m);

}
