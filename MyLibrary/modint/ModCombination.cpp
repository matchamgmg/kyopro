#include <bits/stdc++.h>

template <long long MOD = 998244353> class Modint {
  static_assert(MOD > 0);
  long long value;

  /**
   * @brief value*x + MOD*y = 1 の解 (x, y) のうち，xを返す
   */
  long long inv_impl() const {
    long long x = 1, u = 0, s = value, t = MOD;
    long long k;
    while (t) {
      k = s / t;
      s -= k * t;
      std::swap(s, t);
      x -= k * u;
      std::swap(x, u);
    }
    assert(s == 1);
    x %= MOD;
    if (x < 0) {
      x += MOD;
    }
    return x;
  }

  long long pow_impl(long long x) const {
    long long res = 1;
    long long a;
    if (x >= 0) {
      a = value;
    } else {
      a = inv_impl();
      x *= -1;
    }
    while (x) {
      if (x & 1) {
        res *= a;
        res %= MOD;
      }
      a *= a;
      a %= MOD;
      x >>= 1;
    }
    return res;
  }

public:
  Modint() : value(0) {}

  Modint(long long x) {
    value = x % MOD;
    if (value < 0) {
      value += MOD;
    }
  }

  Modint &operator+=(const Modint &rhs) {
    this->value += rhs.value;
    if (this->value >= MOD) {
      this->value -= MOD;
    }
    return *this;
  }

  Modint operator+(const Modint &rhs) const {
    Modint res = *this;
    res += rhs;
    return res;
  }

  Modint &operator-=(const Modint &rhs) {
    this->value -= rhs.value;
    if (this->value < 0) {
      this->value += MOD;
    }
    return *this;
  }

  Modint operator-(const Modint &rhs) const {
    Modint res = *this;
    res -= rhs;
    return res;
  }

  Modint operator-() const {
    return Modint{MOD - this->value};
  }

  Modint &operator*=(const Modint &rhs) {
    this->value *= rhs.value;
    this->value %= MOD;
    return *this;
  }

  Modint operator*(const Modint &rhs) const {
    Modint res = *this;
    res *= rhs;
    return res;
  }

  Modint &operator/=(const Modint &rhs) {
    this->value *= rhs.inv_impl();
    this->value %= MOD;
    return *this;
  }

  Modint operator/(const Modint &rhs) const {
    Modint res = *this;
    res /= rhs;
    return res;
  }

  long long val() const {
    return value;
  }

  Modint inv() const {
    return Modint{inv_impl()};
  }

  Modint pow(long long x) const {
    return Modint{pow_impl(x)};
  }
};

/**
 * @brief modulo二項係数を計算するクラス（素数mod）
 * https://drken1215.hatenablog.com/entry/2018/06/08/210000
 * 制約 1 <= n <= 1e7，pは素数
 * 階乗の前計算 O(n)
 * 二項係数の計算 O(1)
 */
template <long long MOD = 998244353> class ModCombination {
  using Mint = Modint<MOD>;
  int n;
  std::vector<Mint> inv;
  std::vector<Mint> factorial;
  std::vector<Mint> factorial_inv;

public:
  ModCombination() {}

  ModCombination(int n_)
      : n(n_), inv(std::vector<Mint>(n_ + 1)), factorial(std::vector<Mint>(n_ + 1)),
        factorial_inv(std::vector<Mint>(n_ + 1)) {
    // a^-1 = -(p % a)^-1 * (p / a) を利用
    inv[1] = 1;
    for (int i = 2; i <= n; ++i) {
      inv[i] = -inv[MOD % i] * (MOD / i);
    }

    factorial[0] = 1;
    for (int i = 1; i <= n; ++i) {
      factorial[i] = factorial[i - 1] * i;
    }

    factorial_inv[0] = 1;
    for (int i = 1; i <= n; ++i) {
      factorial_inv[i] = factorial_inv[i - 1] * inv[i];
    }
  }

  Mint Com(int k, int r) {
    assert(0 <= k && k <= n && 0 <= r && r <= n);
    if (k < r) {
      return 0;
    }
    return factorial[k] * factorial_inv[r] * factorial_inv[k - r];
  }

  Mint Inv(int x) {
    assert(0 < x && x <= n);
    return inv[x];
  }
};

/**
 * @brief nが巨大で，固定値のとき（素数mod）
 * comb(n, k) = (n / 1) * ((n-1) / 2) * ... * ((n-k+1) / k) を利用
 * 制約 1 <= n <= 1e9 (固定値), 1 <= k <= 1e7, pは素数
 * 前計算 O(k)
 * 二項係数の計算 O(1)
 */
template <long long MOD = 998244353> class ModCombinationWithBigFixedN {
  using Mint = Modint<MOD>;
  int n;
  int k;
  std::vector<Mint> inv;
  std::vector<Mint> comb;

public:
  ModCombinationWithBigFixedN() {}

  ModCombinationWithBigFixedN(int n_, int k_)
      : n(n_), k(k_), inv(std::vector<Mint>(k_ + 1)), comb(std::vector<Mint>(k_ + 1)) {
    inv[1] = 1;
    for (int i = 2; i <= k; ++i) {
      inv[i] = -inv[MOD % i] * (MOD / i);
    }

    comb[0] = 1;
    for (int i = 1; i <= k; ++i) {
      comb[i] = comb[i - 1] * (n - i + 1) * inv[i];
    }
  }

  Mint Com(int r) {
    assert(0 <= r && r <= k);
    return comb[r];
  }
};

/**
 * @brief nもkも小さいとき
 * 動的計画法により二項係数を計算する
 * 逆元計算が不要
 * 制約 1 <= n <= 5000, 1 <= k <= 5000
 * 前計算 O(n^2)
 * 二項係数の計算 O(1)
 */
template <long long MOD = 998244353> class ModCombinationSmall {
  using Mint = Modint<MOD>;
  int n;
  std::vector<std::vector<Mint>> comb;

public:
  ModCombinationSmall() {}

  ModCombinationSmall(int n_) : n(n_), comb(std::vector<std::vector<Mint>>(n_ + 1, std::vector<Mint>(n_ + 1))) {
    for (int i = 0; i <= n; ++i) {
      comb[i][i] = 1;
    }
    for (int i = 1; i <= n; ++i) {
      comb[i][0] = 1;
      for (int j = 1; j < i; ++j) {
        comb[i][j] = comb[i - 1][j - 1] + comb[i - 1][j];
      }
    }
  }

  Mint Com(int k, int r) {
    assert(0 <= k && k <= n && 0 <= r && r <= n);
    return comb[k][r];
  }
};

using namespace std;

int main() {
  int n = 2e5;
  ModCombination comb(n);
  cout << comb.Com(3, 2).val() << endl;
  cout << comb.Com(100000, 50000).val() << endl;
  cout << comb.Com(10, 3).val() << endl;
  n = 10;
  int k = 2e5;
  ModCombinationWithBigFixedN combN(n, k);
  cout << combN.Com(2).val() << endl;
  ModCombinationSmall combS(1000);
  cout << combS.Com(10, 2).val() << endl;
  cout << combS.Com(10, 11).val() << endl;
}