#include <bits/stdc++.h>

template <long long MOD = 998244353> class Modint {
  static_assert(MOD > 0);
  long long value;

  /**
   * @return d = gcd(a, b)
   * @note a*x + by = gcd(a, b) を満たす解 (x, y) を格納
   */
  //   long long extGCD(long long a, long long b, long long &x, long long &y) {
  //     if (b == 0) {
  //       x = 1;
  //       y = 0;
  //       return a;
  //     }
  //     // b*y + r*x = gcd(a, b) を解く
  //     long long d = extGCD(b, a % b, y, x);
  //     y -= a / b * x;
  //     return d;
  //   }

  //   std::tuple<long long, long long, long long> extGCD(long long a, long long b) {
  //     if (b == 0) {
  //       return {a, 1, 0};
  //     }
  //     const auto [d, x, y] = extGCD(b, a % b);
  //     return {d, y, x - a / b * y};
  //   }

  /**
   * @brief a*x + b*y = gcd(a, b) の解(x, y)を求める
   * https://scol.hatenablog.com/entry/2019/10/19/180000
   */
  //   std::tuple<long long, long long> extGCD(long long a, long long b) const {
  //     long long x = 1, y = 0, u = 0, v = 1, s = a, t = b;
  //     long long k = 0;
  //     while (t) {
  //       k = s / t, s -= k * t;
  //       std::swap(s, t);
  //       x -= k * u;
  //       std::swap(x, u);
  //       y -= k * v;
  //       std::swap(y, v);
  //     }
  //     return {x, y};
  //   }

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

using namespace std;

int main() {
  cout << (-5 / 2) << ' ' << (-5 % 2) << endl;
  Modint<7> a(-1), b(4);
  a += b;
  a = a + b;
  a += 5;
  a = 3;
  a = a.pow(4);
  cout << (a / b).val() << ' ' << (a * b.inv()).val() << endl;
}