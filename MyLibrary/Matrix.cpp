#include <iostream>
#include <vector>
#include <utility>

// n*n 正方行列
template <class T, int size_ = 2>
struct SqMatrix
{
    // 0行列として初期化
    SqMatrix(T x = T{}) : mat(std::vector<T>(size_ * size_, x)) {}

    // xで埋める
    void Fill(const T &x)
    {
        mat.assign(size_ * size_, x);
    }

    // 単位行列化
    void InitIdentity()
    {
        int i = 0;
        mat.assign(size_ * size_, T{});
        while (i < size_ * size_)
        {
            mat[i] = T{1};
            i += size_ + 1;
        }
    }

    // 転置
    SqMatrix<T, size_> Transpose() const
    {
        SqMatrix<T, size_> res;
        for (int i = 0; i < size_; ++i)
        {
            for (int j = 0; j < size_; ++j)
            {
                res.mat[j * size_ + i] = mat[i * size_ + j];
            }
        }
        return res;
    }

    T &operator[](int i, int j) &
    {
        return mat[i * size_ + j];
    }

    // 読み取り専用
    const T &operator[](int i, int j) const &
    {
        return mat[i * size_ + j];
    }

    // 一時オブジェクトに対する呼び出し
    T operator[](int i, int j) const &&
    {
        return std::move(mat[i * size_ + j]);
    }

    T &operator[](int i) &
    {
        return mat[i];
    }

    // 読み取り専用
    const T &operator[](int i) const &
    {
        return mat[i];
    }

    // 一時オブジェクトに対する呼び出し
    T operator[](int i) const &&
    {
        return std::move(mat[i]);
    }

    SqMatrix<T, size_> operator-() const
    {
        SqMatrix<T, size_> res;
        for (int i = 0; i < size_ * size_; ++i)
        {
            res.mat[i] = -mat[i];
        }
        return res;
    }

    /* 四則演算 */

    SqMatrix<T, size_> operator+(const SqMatrix<T, size_> &rhs) const
    {
        SqMatrix<T, size_> res;
        for (int i = 0; i < size_ * size_; ++i)
        {
            res.mat[i] = mat[i] + rhs.mat[i];
        }
        return res;
    }

    SqMatrix<T, size_> operator-(const SqMatrix<T, size_> &rhs) const
    {
        SqMatrix<T, size_> res;
        for (int i = 0; i < size_ * size_; ++i)
        {
            res.mat[i] = mat[i] - rhs.mat[i];
        }
        return res;
    }

    SqMatrix<T, size_> operator*(const SqMatrix<T, size_> &rhs) const
    {
        SqMatrix<T, size_> res;
        for (int i = 0; i < size_; ++i)
        {
            for (int j = 0; j < size_; ++j)
            {
                for (int k = 0; k < size_; ++k)
                {
                    res.mat[i * size_ + j] += mat[i * size_ + k] * rhs.mat[k * size_ + j];
                }
            }
        }
        return res;
    }

    // スカラ倍
    SqMatrix<T, size_> operator*(const T &x) const
    {
        SqMatrix<T, size_> res;
        for (int i = 0; i < size_ * size_; ++i)
        {
            res.mat[i] = x * mat[i];
        }
        return res;
    }

    /* 複合代入 */
    SqMatrix<T, size_> &operator+=(const SqMatrix<T, size_> &rhs) &
    {
        for (int i = 0; i < size_ * size_; ++i)
        {
            mat[i] += rhs.mat[i];
        }
        return *this;
    }

    SqMatrix<T, size_> &operator-=(const SqMatrix<T, size_> &rhs) &
    {
        for (int i = 0; i < size_ * size_; ++i)
        {
            mat[i] -= rhs.mat[i];
        }
        return *this;
    }

    SqMatrix<T, size_> &operator*=(const SqMatrix<T, size_> &rhs) &
    {
        SqMatrix<T, size_> res;
        for (int i = 0; i < size_; ++i)
        {
            for (int j = 0; j < size_; ++j)
            {
                for (int k = 0; k < size_; ++k)
                {
                    res.mat[i * size_ + j] += mat[i * size_ + k] * rhs.mat[k * size_ + j];
                }
            }
        }
        std::swap(res.mat, this->mat);
        return *this;
    }

    SqMatrix<T, size_> &operator*=(const T &x) &
    {
        for (int i = 0; i < size_ * size_; ++i)
        {
            mat[i] *= x;
        }
        return *this;
    }

private:
    std::vector<T> mat;
};

int main()
{
    SqMatrix<int> a, b, c;
    a = b = c;
    int &&x = SqMatrix<int>(5)[0, 0];
    std::cout << x << std::endl;
    a = SqMatrix<int>(1);
    SqMatrix<int> d = SqMatrix<int>(2);
    std::cout << d[0, 0] << std::endl;
    std::cout << (a + d)[0, 0] << std::endl;
    a += d;
    std::cout << a[0, 0] << std::endl;
    b.InitIdentity();
    b *= a;
    std::cout << b[0, 0] << std::endl;
    b *= 3;
    std::cout << b[0, 0] << std::endl;
    a = 1;
    std::cout << a[0, 0] << std::endl;
    a *= a;
    a *= a;
    std::cout << a[0, 0] << a[0, 1] << std::endl;
}