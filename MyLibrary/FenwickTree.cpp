#include <bits/stdc++.h>
using namespace std;

template <class T>
struct BIT
{
    BIT(int n_) : n(n_), data(n_ + 1) {}

    void add(int p, T x = 1)
    {
        for (++p; p <= n; p += p & -p)
        {
            data[p] += x;
        }
    }

    // [0, p)
    T prefix_sum(int p)
    {
        T res = 0;
        for (; p > 0; p -= p & -p)
        {
            res += data[p];
        }
        return res;
    }

    // [l, r)
    T sum(int l, int r)
    {
        return prefix_sum(r) - prefix_sum(l);
    }

    // binary_search
    // sum[0,k] >= v となる最小のkを返す
    // 存在しない場合はn
    int lower_bound(T v)
    {
        int w = 1, i = 0;
        while (w < n)
        {
            w <<= 1;
        }
        for (++w; w > 0; w >>= 1)
        {
            if (i + w > n)
                continue;
            if (data[i + w] < v)
                v -= data[i += w];
        }
        return i;
    }

    // sum[0,k] > v となる最小のkを返す
    int upper_bound(T v)
    {
        return lower_bound(v + 1);
    }

    // sum[0,k] >= v となる最小のkを返す
    int min_left(T v)
    {
        return lower_bound(v);
    }

    // sum[0, k] <= v となる最大のkを返す
    // 存在しない場合は-1
    int max_right(T v)
    {
        return lower_bound(v + 1) - 1;
    }

    // K番目の要素を取得
    int get_Kth_element(T k)
    {
        return lower_bound(k);
    }

private:
    int n;
    std::vector<T> data;
};

int main()
{
    int N = 10;
    BIT<int> ft(N);
    for (int i = 1; i < N; ++i)
    {
        ft.add(i);
    }
    for (int i = 1; i <= N; ++i)
    {
        std::cout << ft.sum(i) << ' ';
    }
    std::cout << std::endl;
    for (int i = 0; i < N * N; ++i)
    {
        std::cout << ft.lower_bound(i) << ' ';
    }
    std::cout << std::endl;
    BIT<int> ft2(N);
    ft2.add(0, 1);
    ft2.add(1, 1);
    ft2.add(2, 2);
    ft2.add(3, 2);
    ft2.add(4, 3);
    ft2.add(5, 0);
    ft2.add(6, 2);
    ft2.add(7, 2);
    vector<int> A = {0, 1, 2, 2, 3, 3, 4, 4, 4, 6, 6, 7, 7};
    for (int a : A)
    {
        std::cout << a << ' ';
    }
    std::cout << std::endl;
    std::cout << "sum" << std::endl;
    for (int k = 0; k <= 13; ++k)
    {
        std::cout << ft2.sum(k + 1) << ' ';
    }
    std::cout << std::endl;
    std::cout << "upper_bound" << std::endl;
    for (int k = 0; k <= 13; ++k)
    {
        std::cout << ft2.upper_bound(k) << ' ';
    }
    std::cout << std::endl;
    std::cout << "lower_bound" << std::endl;
    for (int k = 0; k <= 13; ++k)
    {
        std::cout << ft2.lower_bound(k) << ' ';
    }
    std::cout << std::endl;
    std::cout << "max_right" << std::endl;
    for (int k = 0; k <= 13; ++k)
    {
        std::cout << ft2.max_right(k) << ' ';
    }
}