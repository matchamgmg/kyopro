#include <bits/stdc++.h>

#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
using mint = modint998244353;
// using mint = modint1000000007;
template <class T>
using pq = priority_queue<T, vector<T>>; // 大きい順
template <class T>
using pq_g = priority_queue<T, vector<T>, greater<T>>; // 小さい順

#define rep(i, s, n) for (int i = s; i < (int)n; i++)
#define rrep(i, s, n) for (int i = (int)n - 1; i >= s; i--)
#define all(a) a.begin(), a.end()
#define rall(a) a.rbegin(), a.rend()
void pyes() { cout << "Yes" << endl; }
void pno() { cout << "No" << endl; }
void YN(bool x)
{
    cout << (x ? "Yes" : "No") << endl;
}
template <class T>
void v_cout(const vector<T> &a)
{
    int n = a.size();
    rep(i, 0, n) cout << a[i] << " ";
    cout << endl;
}
template <class T>
void vv_cout(const vector<T> &a)
{
    int n = a.size();
    rep(i, 0, n)
    {
        rep(j, 0, a[i].size()) cout << a[i][j] << " ";
        cout << endl;
    }
}
bool grid_check(int x, int y, int X, int Y)
{
    return (0 <= x && x < X && 0 <= y && y < Y);
}
template <class T>
bool chmax(T &a, T b)
{
    if (a < b)
    {
        a = b;
        return true;
    }
    else
    {
        return false;
    }
}
template <class T>
bool chmin(T &a, T b)
{
    if (a > b)
    {
        a = b;
        return true;
    }
    else
    {
        return false;
    }
}

// S_{infty} の Z 番目の文字をC_{infty}(Z)とおく
// |S_{n}| < Z を満たす最大のnについて
// C_{infty}(Z) = C_{n-1}(Z-|S_{n}|)

// S_{n}を求める

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

    static SqMatrix<T, size_> GetIdentity()
    {
        SqMatrix<T, size_> res;
        int i = 0;
        while (i < size_ * size_)
        {
            res.mat[i] = T{1};
            i += size_ + 1;
        }
        return res;
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
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    using Answer = array<ll, 26>;
    Answer ans0;
    rep(i, 0, 26)
    {
        ans0[i] = 0;
    }
    auto Add_answer = [](Answer &l, const Answer &r) -> void
    {
        rep(i, 0, 26)
        {
            l[i] += r[i];
        }
    };
    auto Diff_answer = [](Answer &l, const Answer &r) -> void
    {
        rep(i, 0, 26)
        {
            l[i] -= r[i];
        }
    };

    string X, Y;
    cin >> X >> Y;
    ll len_max = 1e18;
    ll x = X.size(), y = Y.size();

    vector<SqMatrix<ll>> F;
    SqMatrix<ll> Fi = SqMatrix<ll>::GetIdentity();
    SqMatrix<ll> F1(1);
    F1[1, 1] = 0;

    F.emplace_back(Fi);
    while (Fi[1, 0] * x <= len_max)
    {
        Fi *= F1;
        F.emplace_back(Fi);
    }
    ll N = F.size() - 1;

    vector<Answer> ansX(x + 1), ansY(y + 1);
    rep(i, 0, x)
    {
        rep(c, 0, 26)
        {
            ansX[i + 1][c] += ansX[i][c];
        }
        ansX[i + 1][X[i] - 'a']++;
    }
    rep(i, 0, y)
    {
        rep(c, 0, 26)
        {
            ansY[i + 1][c] += ansY[i][c];
        }
        ansY[i + 1][Y[i] - 'a']++;
    }

    SqMatrix<ll> F1_inv;
    F1_inv[0, 1] = 1;
    F1_inv[1, 0] = 1;
    F1_inv[1, 1] = -1;
    SqMatrix<ll> F2_inv = F1_inv * F1_inv;

    map<ll, Answer> memo;
    auto solve = [&](auto f, ll n, ll Z) -> Answer
    {
        // Z < |F_{n}| なるnを求める
        // sub_solve(n, z) := F_{n} の z 文字目までについて各文字の個数を求める
        // solve(Z) = sub_solve(n, Z) = sub_solve(n-1, |F_{n-1}|) + solve(Z-|F_{n-1}|);
        Answer res = ans0;
        if (Z == 0)
        {
            return res;
        }
        else if (Z <= y)
        {
            Add_answer(res, ansY[Z]);
            return res;
        }
        else if (Z <= y + x)
        {
            Add_answer(res, ansY[y]);
            Add_answer(res, ansX[Z - y]);
            return res;
        }

        SqMatrix<ll> Fn = F[n - 2];
        while (Fn[0, 0] * y + Fn[0, 1] * x > Z)
        {
            Fn *= F1_inv;
            --n;
        }

        // n >= 3
        ll len_Fn = Fn[0, 0] * y + Fn[0, 1] * x;
        ll len_rest = Z - len_Fn;
        if (!memo.contains(n - 1))
        {
            memo[n - 1] = f(f, n - 1, len_Fn);
        }
        Add_answer(res, memo[n - 1]);
        Add_answer(res, f(f, n - 2, len_rest));
        return res;
    };

    int Q;
    cin >> Q;
    rep(q, 0, Q)
    {
        ll L, R;
        char C;
        cin >> L >> R >> C;
        L--;
        Answer ansR = solve(solve, N, R), ansL = solve(solve, N, L);
        cout << ansR[C - 'a'] - ansL[C - 'a'] << endl;
    }
}