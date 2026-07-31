#include <bits/stdc++.h>
using namespace std;

//==============================================================================
// Constants and scalar utilities
//==============================================================================
namespace Vec2D {
using Real = long double;

inline constexpr Real EPS = 1e-12L;
inline constexpr Real PI = std::numbers::pi_v<Real>;

/**
 * @brief 値の符号をEPSを考慮して判定する
 *
 * @return
 *   1: value > 0
 *   0: value == 0
 *  -1: value < 0
 */

constexpr int sign(Real value) noexcept {
  if (value > EPS) {
    return 1;
  } else if (value < -EPS) {
    return -1;
  } else {
    return 0;
  }
}

/**
 * @brief 値が0に十分近いか判定する
 */
constexpr bool is_zero(Real value) noexcept { return sign(value) == 0; }

/**
 * @brief 2つの値が十分近いか判定する
 */
constexpr bool almost_equal(Real lhs, Real rhs) noexcept { return is_zero(lhs - rhs); }

/**
 * @brief lhs < rhs をEPSを考慮して判定する
 */
constexpr bool less_than(Real lhs, Real rhs) noexcept { return lhs < rhs - EPS; }

/**
 * @brief lhs <= rhs をEPSを考慮して判定する
 */
constexpr bool less_equal(Real lhs, Real rhs) noexcept { return lhs <= rhs + EPS; }

/**
 * @brief lhs > rhs をEPSを考慮して判定する
 */
constexpr bool greater_than(Real lhs, Real rhs) noexcept { return lhs > rhs + EPS; }

/**
 * @brief lhs >= rhs をEPSを考慮して判定する
 */
constexpr bool greater_equal(Real lhs, Real rhs) noexcept { return lhs >= rhs - EPS; }

//==============================================================================
// Point2D
//==============================================================================

struct Point2D {
  Real x{};
  Real y{};

  constexpr Point2D() = default;

  constexpr Point2D(Real x_, Real y_) noexcept : x(x_), y(y_) {}

  constexpr Point2D operator-() const noexcept { return {-x, -y}; }

  constexpr Point2D &operator+=(const Point2D &rhs) noexcept {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }

  constexpr Point2D &operator-=(const Point2D &rhs) noexcept {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
  }

  constexpr Point2D &operator*=(Real scalar) noexcept {
    x *= scalar;
    y *= scalar;
    return *this;
  }

  constexpr Point2D &operator/=(Real scalar) {
    assert(!is_zero(scalar));

    x /= scalar;
    y /= scalar;
    return *this;
  }
};

//==============================================================================
// Arithmetic operators
//==============================================================================

constexpr Point2D operator+(Point2D lhs, const Point2D &rhs) noexcept {
  lhs += rhs;
  return lhs;
}

constexpr Point2D operator-(Point2D lhs, const Point2D &rhs) noexcept {
  lhs -= rhs;
  return lhs;
}

constexpr Point2D operator*(Point2D point, Real scalar) noexcept {
  point *= scalar;
  return point;
}

constexpr Point2D operator*(Real scalar, Point2D point) noexcept {
  point *= scalar;
  return point;
}

constexpr Point2D operator/(Point2D point, Real scalar) {
  point /= scalar;
  return point;
}

//==============================================================================
// Comparison
//==============================================================================

/**
 * @brief 2点が幾何学的に同じ位置にあるか判定する
 */
constexpr bool almost_equal(const Point2D &lhs, const Point2D &rhs) noexcept {
  return almost_equal(lhs.x, rhs.x) && almost_equal(lhs.y, rhs.y);
}

/**
 * @brief 辞書順による厳密な比較
 *
 * std::sortやstd::setなどで使用する。
 *
 * EPSを使用すると推移律が壊れ、strict weak orderingを満たさなくなる
 * 可能性があるため、この比較ではEPSを使用しない。
 *
 * 使用例
 *  LexicographicalLess comp;
 *  bool ok = comp(a, b);
 *
 *  sort(A.begin(), A.end(), LexicographicalLess{});
 */
struct LexicographicalLess {
  constexpr bool operator()(const Point2D &lhs, const Point2D &rhs) const noexcept {
    return (lhs.x == rhs.x ? lhs.y < rhs.y : lhs.x < rhs.x);
  }
};

//==============================================================================
// Stream operators
//==============================================================================

inline std::istream &operator>>(std::istream &is, Point2D &point) { return is >> point.x >> point.y; }

inline std::ostream &operator<<(std::ostream &os, const Point2D &point) {
  return os << '(' << point.x << ", " << point.y << ')';
}

//==============================================================================
// Basic vector operations
//==============================================================================

/**
 * @brief 2つのベクトルの内積を求める
 */
constexpr Real dot(const Point2D &lhs, const Point2D &rhs) noexcept { return lhs.x * rhs.x + lhs.y * rhs.y; }

/**
 * @brief 2つのベクトルの外積を求める
 *
 * 2次元ベクトルを3次元ベクトルとみなしたときの、
 * 外積のz成分を返す。
 */
constexpr Real cross(const Point2D &lhs, const Point2D &rhs) noexcept { return lhs.x * rhs.y - lhs.y * rhs.x; }

/**
 * @brief 3点A, B, Cに対してABとACの外積を求める
 */
constexpr Real cross(const Point2D &a, const Point2D &b, const Point2D &c) noexcept { return cross(b - a, c - a); }

/**
 * @brief ベクトルの長さの二乗を求める
 */
constexpr Real norm2(const Point2D &vector) noexcept { return dot(vector, vector); }

/**
 * @brief ベクトルの長さを求める
 *
 * std::hypot(x, y) ... sqrt(x*x + y*y) より安全らしい
 */
inline Real norm(const Point2D &vector) noexcept { return std::hypot(vector.x, vector.y); }

/**
 * @brief 2点間の距離の二乗を求める
 */
constexpr Real distance2(const Point2D &lhs, const Point2D &rhs) noexcept { return norm2(lhs - rhs); }

/**
 * @brief 2点間の距離を求める
 */
inline Real distance(const Point2D &lhs, const Point2D &rhs) noexcept { return norm(lhs - rhs); }

/**
 * @brief ベクトルがゼロベクトルか判定する
 */
constexpr bool is_zero(const Point2D &vector) noexcept { return is_zero(norm2(vector)); }

/**
 * @brief ベクトルの偏角を求める
 *
 * 戻り値の範囲は[-PI, PI]。
 */
inline Real argument(const Point2D &vector) noexcept { return std::atan2(vector.y, vector.x); }

/**
 * @brief ベクトルを単位ベクトルに変換する
 *
 * ゼロベクトルに対して呼び出してはならない。
 */
inline Point2D normalized(const Point2D &vector) {
  const Real length = norm(vector);
  assert(!is_zero(length));
  return vector / length;
}

//==============================================================================
// Rotation
//==============================================================================

/**
 * @brief ベクトルを反時計回りに90度回転する
 */
constexpr Point2D rotate90(const Point2D &vector) noexcept { return {-vector.y, vector.x}; }

/**
 * @brief ベクトルを時計回りに90度回転する
 */
constexpr Point2D rotate_minus90(const Point2D &vector) noexcept { return {vector.y, -vector.x}; }

/**
 * @brief ベクトルを反時計回りにthetaラジアン回転する
 */
inline Point2D rotate(const Point2D &vector, Real theta) noexcept {
  const Real cosine = std::cos(theta);
  const Real sine = std::sin(theta);

  return {
      cosine * vector.x - sine * vector.y,
      sine * vector.x + cosine * vector.y,
  };
}

/**
 * @brief 点をcenterを中心として反時計回りに回転する
 */
inline Point2D rotate(const Point2D &point, const Point2D &center, Real theta) noexcept {
  return center + rotate(point - center, theta);
}

//==============================================================================
// Angle
//==============================================================================

/**
 * @brief 2つのベクトルのなす角を求める
 *
 * 戻り値は[0, PI]。
 * ゼロベクトルに対して呼び出してはならない。
 *
 * std::clamp(cosine, -1.0L, 1.0L) ... -1 <= cosine <= 1 に丸め
 */
inline Real angle(const Point2D &lhs, const Point2D &rhs) {
  const Real lhs_norm = norm(lhs);
  const Real rhs_norm = norm(rhs);

  assert(!is_zero(lhs_norm));
  assert(!is_zero(rhs_norm));

  Real cosine = dot(lhs, rhs) / (lhs_norm * rhs_norm);
  cosine = std::clamp(cosine, -1.0L, 1.0L);

  return std::acos(cosine);
}

/**
 * @brief ベクトルfromからtoへの符号付き回転角を求める
 *
 * 戻り値の範囲は[-PI, PI]。
 * 正なら反時計回り、負なら時計回り。
 *
 * cross(a, b) = |a||b|sin(theta)
 * dot(a, b) = |a||b|cos(theta)
 * atan(sin(theta), cos(theta)) = theta
 */
inline Real signed_angle(const Point2D &from, const Point2D &to) noexcept {
  return std::atan2(cross(from, to), dot(from, to));
}

//==============================================================================
// Projection
//==============================================================================

/**
 * @brief ベクトルvectorをbase方向へ正射影する
 *
 * baseがゼロベクトルの場合は呼び出してはならない。
 */
inline Point2D projection(const Point2D &vector, const Point2D &base) {
  const Real base_norm2 = norm2(base);
  assert(!is_zero(base_norm2));
  return base * (dot(vector, base) / base_norm2);
}

/**
 * @brief ベクトルvectorのbaseに垂直な成分を求める
 */
inline Point2D rejection(const Point2D &vector, const Point2D &base) { return vector - projection(vector, base); }

//==============================================================================
// Relationship between vectors
//==============================================================================

/**
 * @brief 2つのベクトルが平行か判定する
 */
constexpr bool is_parallel(const Point2D &lhs, const Point2D &rhs) noexcept { return is_zero(cross(lhs, rhs)); }

/**
 * @brief 2つのベクトルが直交しているか判定する
 */
constexpr bool is_orthogonal(const Point2D &lhs, const Point2D &rhs) noexcept { return is_zero(dot(lhs, rhs)); }

//==============================================================================
// CCW (Counter Clockwise)
//==============================================================================

enum class CCW {
  Clockwise = -1,
  OnSegment = 0,
  CounterClockwise = 1,
  OnlineBack = 2,
  OnlineFront = -2,
};

/**
 * @brief 有向直線ABに対する点Cの位置関係を判定する
 */
constexpr CCW ccw(const Point2D &a, const Point2D &b, const Point2D &c) noexcept {
  const Point2D ab = b - a;
  const Point2D ac = c - a;

  const int cross_sign = sign(cross(ab, ac));

  if (cross_sign > 0) {
    return CCW::CounterClockwise;
  }

  if (cross_sign < 0) {
    return CCW::Clockwise;
  }

  if (less_than(dot(ab, ac), 0.0L)) {
    return CCW::OnlineBack;
  }

  if (less_than(norm2(ab), norm2(ac))) {
    return CCW::OnlineFront;
  }

  return CCW::OnSegment;
}

/**
 * @brief 点が線分AB上にあるか判定する
 */
constexpr bool is_on_segment(const Point2D &point, const Point2D &a, const Point2D &b) noexcept {
  return ccw(a, b, point) == CCW::OnSegment;
}

//===================================================
// Line, Segment
//===================================================

/**
 * @brief 直線
 */
struct Line {
  Point2D a;
  Point2D b;
};

/**
 * @brief 線分
 */
struct Segment {
  Point2D a;
  Point2D b;
};

/**
 * @brief 点pointを直線lineへ正射影した点を求める
 *
 * line.aとline.bが同一店の場合は呼び出してはならない。
 */
inline Point2D projection(const Point2D &point, const Line &line) {
  const Point2D direction = line.b - line.a;
  assert(!is_zero(direction));

  return line.a + projection(point - line.a, direction);
}

/**
 * @brief 点pointに最も近い線分segment上の点を求める
 */
inline Point2D closest_point(const Point2D &point, const Segment &segment) {
  const Point2D direction = segment.b - segment.a;
  const Real direction_norm2 = norm2(direction);

  if (is_zero(direction_norm2)) {
    return segment.a;
  }

  Real t = dot(point - segment.a, direction) / direction_norm2;
  t = std::clamp(t, Real(0), Real(1));

  return segment.a + direction * t;
}

/**
 * @brief 点pointと直線lineとの距離を求める
 */
inline Real distance(const Point2D &point, const Line &line) { return distance(point, projection(point, line)); }

/**
 * @brief 点pointと線分segmentとの距離を求める
 */
inline Real distance(const Point2D &point, const Segment &segment) {
  return distance(point, closest_point(point, segment));
}

}; // namespace Vec2D

using namespace Vec2D;
int main() {
  cout << fixed << setprecision(10) << PI << endl;
  cout << sign(-0.0) << endl;
  Point2D A(1, 1), B(2, 2), C(1, 2);
  Point2D X = A + B;
  cout << A.x << ' ' << A.y << endl;
}