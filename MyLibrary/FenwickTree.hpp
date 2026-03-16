/*
Fenwick Tree (BIT)

N要素の数列Aについて
基本機能:
    一点加算 O(logN)
    区間和取得 O(logN)
追加機能:
    prefix_sumの二分探索 O(logN)
    定義域[0,N-1]の整数を要素とする，順序付き多重集合におけるK番目(K >= 1)の要素取得 O(logN)


functions:
    add(pos:int, value:T) -> void : A[pos] に value を加算
    prefix_sum(r:int) -> T : 数列Aの [0, r) の区間和を取得
    sum(l:int, r:int) -> T : 数列Aの [l, r) の区間和を取得

    数列Aの区間 [0, k] の区間和を A[0, k] とする
    lower_bound(value:T) -> int : A[0, k] >= value を満たす最小のkを取得
    upper_bound(value:T) -> int : A[0, k] >  value を満たす最小のkを取得
    min_left(value:T) -> int    : A[0, k] >= value を満たす最小のkを取得
    max_right(value:T) -> int   : A[0, k] >= value を満たす最大のkを取得

    定義域[0,N-1]の整数を要素とする，順序付き多重集合Sを考える
    A[i] : Sにおける要素iの個数
    get_Kth_element(k:T) -> int : Sのk番目の要素の取得
*/

#include <vector>

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