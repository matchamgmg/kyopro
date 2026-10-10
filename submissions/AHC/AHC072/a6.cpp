/**
 * https://github.com/matchamgmg/kyopro/tree/main
 */

#include <atcoder/all>
#include <bits/stdc++.h>

using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
using mint = modint998244353;
// using mint = modint1000000007;
template <class T> using pq = priority_queue<T, vector<T>>;               // 大きい順
template <class T> using pq_g = priority_queue<T, vector<T>, greater<T>>; // 小さい順

#define rep(i, s, n) for (int i = (s); i < (int)(n); i++)
#define rrep(i, s, n) for (int i = (n - 1); i >= (int)(s); i--)
#define all(a) a.begin(), a.end()
#define rall(a) a.rbegin(), a.rend()
void pyes() {
  cout << "Yes" << endl;
}
void pno() {
  cout << "No" << endl;
}
void YN(bool x) {
  cout << (x ? "Yes" : "No") << endl;
}
template <class T> void v_cout(const vector<T> &a) {
  int n = a.size();
  rep(i, 0, n) cout << a[i] << " ";
  cout << endl;
}
template <class T> void vv_cout(const vector<T> &a) {
  int n = a.size();
  rep(i, 0, n) {
    rep(j, 0, a[i].size()) cout << a[i][j] << " ";
    cout << endl;
  }
}
bool grid_check(int x, int y, int X, int Y) {
  return (0 <= x && x < X && 0 <= y && y < Y);
}
template <class T> bool chmax(T &a, T b) {
  if (a < b) {
    a = b;
    return true;
  } else {
    return false;
  }
}
template <class T> bool chmin(T &a, T b) {
  if (a > b) {
    a = b;
    return true;
  } else {
    return false;
  }
}

class TimeKeeper {
private:
  std::chrono::high_resolution_clock::time_point start_time_;
  int64_t time_threshold_;

public:
  TimeKeeper() {};

  TimeKeeper(const int64_t &time_threshold)
      : start_time_(std::chrono::high_resolution_clock::now()), time_threshold_(time_threshold) {}

  bool isTimeOver() const {
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;
    auto diff = std::chrono::high_resolution_clock::now() - this->start_time_;

    return duration_cast<milliseconds>(diff).count() >= time_threshold_;
  }

  auto get_time() const {
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;
    auto diff = std::chrono::high_resolution_clock::now() - this->start_time_;
    return duration_cast<milliseconds>(diff).count();
  }
};

const int INF = 1e9;
array<char, 4> UDLR = {'U', 'D', 'L', 'R'};
array<int, 4> dx = {-1, 1, 0, 0}, dy = {0, 0, -1, 1};

struct Problem {
  int N;
  int K;
  vector<string> C;
  Problem() {
    input();
  }
  void input() {
    cin >> N >> K;
    C.resize(N);
    for (string &s : C) {
      cin >> s;
    }
  }
};

struct Legal_action {
  int x;
  int y;
  int k;
  int d;
  int l;
  friend ostream &operator<<(ostream &os, const Legal_action &ac) {
    cout << ac.x << ' ' << ac.y << ' ' << ac.k << ' ' << UDLR[ac.d] << ' ' << ac.l;
    return os;
  }
};

struct Answer {
  vector<Legal_action> actions;
  void print() {
    for (Legal_action &action : actions) {
      cout << action << '\n';
    }
  }
};

struct SlimeInfo {
  int id;
  int color;
  int initial_x;
  int initial_y;
};

struct ResolvedSlime {
  int id;
  int x;
  int y;
};

struct MaterialCandidate {
  int id;
  int color;

  int gather_cost;
  int nest_cost;
  int original_cost;
  int detour_cost;

  bool on_shortest_path;
};

struct MaterialPlan {
  int color = -1;
  vector<MaterialCandidate> slimes;

  int gather_cost = INF;
  int detour_cost = INF;

  bool valid() const {
    return color != -1;
  }
};

struct HighwaySegment {
  int sx, sy; // 始点
  int ex, ey; // 終点
  int dir;    // UDLR の方向
  int length; // マス数
};

struct JumpPlatform {
  int x, y;
  int dir;
  int height;

  int traffic;
  int saving;
  MaterialPlan material;

  int value() const {
    if (!material.valid()) {
      return -INF;
    }
    return saving - material.detour_cost;
  }
};

struct FieldFeature {
  int floor_count = 0;
  int wall_count = 0;
  int slime_count = 0;

  double slime_density = 0.0;
  double wall_density = 0.0;

  // そのマスを通るスライム数
  vector<vector<int>> traffic;
  // traffic_dir[x][y][d]
  // (x, y) から d 方向へ進むスライム数
  vector<vector<array<int, 4>>> traffic_dir;
  int max_traffic = 0;

  vector<vector<bool>> highway;

  vector<HighwaySegment> highway_segments;
  vector<JumpPlatform> jump_platforms;
};

struct Solver {
  Problem P;
  vector<SlimeInfo> slime_info;
  vector<vector<vector<int>>> Dist;
  FieldFeature feature;
  mt19937 mt = mt19937{0};
  TimeKeeper tk = TimeKeeper{1900};

  Solver() : Dist(P.K, vector<vector<int>>(P.N, vector<int>(P.N, INF))) {
    initialize_slime_info();
    calc_all_color_distance();
    calc_field_feature();
    // print_traffic();
    calc_highway(0.1);
    // print_highway();
    calc_highway_segments();
    calc_jump_platform_candidates();
  }

  void initialize_slime_info() {
    slime_info.clear();
    int id = 0;
    for (int x = 0; x < P.N; ++x) {
      for (int y = 0; y < P.N; ++y) {
        char c = P.C[x][y];
        if ('a' <= c && c <= 'l') {
          slime_info.push_back(SlimeInfo{
              id,
              c - 'a',
              x,
              y,
          });
          ++id;
        }
      }
    }
  }

  int slime_color(int id) const {
    return slime_info[id].color;
  }

  void calc_all_color_distance() {
    vector<pair<int, int>> nest(P.K);
    for (int i = 0; i < P.N; ++i) {
      for (int j = 0; j < P.N; ++j) {
        char c = P.C[i][j];
        if ('A' <= c && c <= 'L') {
          nest[c - 'A'] = {i, j};
        }
      }
    }
    for (int color = 0; color < P.K; ++color) {
      queue<pair<int, int>> que;
      auto [sx, sy] = nest[color];
      Dist[color][sx][sy] = 0;
      que.emplace(sx, sy);
      while (!que.empty()) {
        auto [x, y] = que.front();
        que.pop();
        for (int d = 0; d < 4; ++d) {
          int nx = x + dx[d];
          int ny = y + dy[d];
          if (!grid_check(nx, ny, P.N, P.N)) {
            continue;
          }
          if (P.C[nx][ny] == '#') {
            continue;
          }
          if (Dist[color][nx][ny] != INF) {
            continue;
          }
          Dist[color][nx][ny] = Dist[color][x][y] + 1;
          que.emplace(nx, ny);
        }
      }
    }
  }

  vector<vector<int>> calc_distance_from(int sx, int sy) const {
    vector dist(P.N, vector<int>(P.N, INF));
    queue<pair<int, int>> que;
    dist[sx][sy] = 0;
    que.emplace(sx, sy);
    while (!que.empty()) {
      auto [x, y] = que.front();
      que.pop();
      for (int d = 0; d < 4; ++d) {
        int nx = x + dx[d];
        int ny = y + dy[d];
        if (!grid_check(nx, ny, P.N, P.N)) {
          continue;
        }
        if (P.C[nx][ny] == '#') {
          continue;
        }
        if (dist[nx][ny] != INF) {
          continue;
        }
        dist[nx][ny] = dist[x][y] + 1;
        que.emplace(nx, ny);
      }
    }
    return dist;
  }

  void calc_field_feature() {
    feature = FieldFeature();
    feature.traffic.assign(P.N, vector<int>(P.N, 0));
    feature.traffic_dir.assign(P.N, vector<array<int, 4>>(P.N, array<int, 4>{}));

    // 基本情報
    for (int x = 0; x < P.N; ++x) {
      for (int y = 0; y < P.N; ++y) {
        char c = P.C[x][y];
        if (c == '#') {
          ++feature.wall_count;
          continue;
        }
        ++feature.floor_count;
        if ('a' <= c && c <= 'l') {
          ++feature.slime_count;
        }
      }
    }
    int non_nest_floor = feature.floor_count - P.K;
    if (non_nest_floor > 0) {
      feature.slime_density = static_cast<double>(feature.slime_count) / non_nest_floor;
    }
    feature.wall_density = static_cast<double>(feature.wall_count) / (P.N * P.N);

    // 各スライムについて1本の最短経路を復元
    for (int sx = 0; sx < P.N; ++sx) {
      for (int sy = 0; sy < P.N; ++sy) {
        char c = P.C[sx][sy];
        if (!(c >= 'a' && c <= 'l')) {
          continue;
        }
        int color = c - 'a';
        int x = sx;
        int y = sy;
        ++feature.traffic[x][y];
        while (Dist[color][x][y] > 0) {
          bool found = false;
          for (int d = 0; d < 4; ++d) {
            int nx = x + dx[d];
            int ny = y + dy[d];
            if (!grid_check(nx, ny, P.N, P.N)) {
              continue;
            }
            if (Dist[color][nx][ny] != Dist[color][x][y] - 1) {
              continue;
            }
            // (x,y) から d 方向へ進む
            ++feature.traffic_dir[x][y][d];
            x = nx;
            y = ny;
            ++feature.traffic[x][y];
            found = true;
            break;
          }
          assert(found);
        }
      }
    }

    for (int x = 0; x < P.N; ++x) {
      for (int y = 0; y < P.N; ++y) {
        feature.max_traffic = max(feature.max_traffic, feature.traffic[x][y]);
      }
    }
  }

  void print_traffic() const {
    cerr << "traffic:\n";
    for (int x = 0; x < P.N; ++x) {
      for (int y = 0; y < P.N; ++y) {
        if (P.C[x][y] == '#') {
          cerr << setw(4) << '#';
        } else {
          cerr << setw(4) << feature.traffic[x][y];
        }
      }
      cerr << '\n';
    }
    cerr << "max_traffic: " << feature.max_traffic << '\n';
  }

  // 暫定版
  void calc_highway(double ratio) {
    feature.highway.assign(P.N, vector<bool>(P.N, false));

    int threshold = max(2, static_cast<int>(ceil(feature.slime_count * ratio)));

    for (int x = 0; x < P.N; ++x) {
      for (int y = 0; y < P.N; ++y) {
        if (P.C[x][y] == '#') {
          continue;
        }

        if (feature.traffic[x][y] >= threshold) {
          feature.highway[x][y] = true;
        }
      }
    }
  }

  void print_highway() const {
    cerr << "highway:\n";

    for (int x = 0; x < P.N; ++x) {
      for (int y = 0; y < P.N; ++y) {
        if (P.C[x][y] == '#') {
          cerr << '#';
        } else if (feature.highway[x][y]) {
          cerr << 'H';
        } else {
          cerr << '.';
        }
      }
      cerr << '\n';
    }
  }

  void calc_highway_segments() {
    feature.highway_segments.clear();
    // 横方向
    for (int x = 0; x < P.N; ++x) {
      int y = 0;
      while (y < P.N) {
        if (!feature.highway[x][y]) {
          ++y;
          continue;
        }
        int sy = y;
        while (y + 1 < P.N && feature.highway[x][y + 1]) {
          ++y;
        }
        int ey = y;
        if (ey - sy + 1 >= 2) {
          int right_traffic = 0;
          int left_traffic = 0;
          for (int j = sy; j < ey; ++j) {
            right_traffic += feature.traffic_dir[x][j][3];
          }
          for (int j = sy + 1; j <= ey; ++j) {
            left_traffic += feature.traffic_dir[x][j][2];
          }
          if (right_traffic > 0) {
            feature.highway_segments.push_back(HighwaySegment{
                x,
                sy,
                x,
                ey,
                3,
                ey - sy + 1,
            });
          }
          if (left_traffic > 0) {
            feature.highway_segments.push_back(HighwaySegment{
                x,
                ey,
                x,
                sy,
                2,
                ey - sy + 1,
            });
          }
        }
        ++y;
      }
    }
    // 縦方向
    for (int y = 0; y < P.N; ++y) {
      int x = 0;
      while (x < P.N) {
        if (!feature.highway[x][y]) {
          ++x;
          continue;
        }
        int sx = x;
        while (x + 1 < P.N && feature.highway[x + 1][y]) {
          ++x;
        }
        int ex = x;
        if (ex - sx + 1 >= 2) {
          int down_traffic = 0;
          int up_traffic = 0;
          for (int i = sx; i < ex; ++i) {
            down_traffic += feature.traffic_dir[i][y][1];
          }
          for (int i = sx + 1; i <= ex; ++i) {
            up_traffic += feature.traffic_dir[i][y][0];
          }
          if (down_traffic > 0) {
            feature.highway_segments.push_back(HighwaySegment{
                sx,
                y,
                ex,
                y,
                1,
                ex - sx + 1,
            });
          }
          if (up_traffic > 0) {
            feature.highway_segments.push_back(HighwaySegment{
                ex,
                y,
                sx,
                y,
                0,
                ex - sx + 1,
            });
          }
        }
        ++x;
      }
    }
  }

  bool is_nest(int x, int y) const {
    return 'A' <= P.C[x][y] && P.C[x][y] <= 'L';
  }

  void calc_jump_platform_candidates() {
    feature.jump_platforms.clear();
    for (const auto &seg : feature.highway_segments) {
      for (int x = seg.sx, y = seg.sy;; x += dx[seg.dir], y += dy[seg.dir]) {
        if (!is_nest(x, y)) {
          for (int height = 2; height <= 4; ++height) {
            int nx = x + dx[seg.dir] * height;
            int ny = y + dy[seg.dir] * height;
            if (!grid_check(nx, ny, P.N, P.N)) {
              continue;
            }
            bool ok = true;
            for (int l = 1; l <= height; ++l) {
              int tx = x + dx[seg.dir] * l;
              int ty = y + dy[seg.dir] * l;
              if (!feature.highway[tx][ty]) {
                ok = false;
                break;
              }
            }
            if (!ok) {
              continue;
            }
            int traffic = INF;
            for (int l = 0; l < height; ++l) {
              int tx = x + dx[seg.dir] * l;
              int ty = y + dy[seg.dir] * l;
              traffic = min(traffic, feature.traffic_dir[tx][ty][seg.dir]);
            }
            if (traffic == 0) {
              continue;
            }
            int saving = traffic * (height - 1);
            auto material = evaluate_materials(x, y, height);
            if (!material.valid()) {
              continue;
            }
            feature.jump_platforms.push_back(JumpPlatform{
                x,
                y,
                seg.dir,
                height,
                traffic,
                saving,
                std::move(material),
            });
          }
        }
        if (x == seg.ex && y == seg.ey) {
          break;
        }
      }
    }
  }

  void print_jump_platform_candidates() const {
    cerr << "jump platform candidates:\n";
    for (const auto &p : feature.jump_platforms) {
      cerr << "(" << p.x << ", " << p.y << ")"
           << " dir=" << UDLR[p.dir] << " h=" << p.height << " traffic=" << p.traffic << " saving=" << p.saving
           << " material_color=" << char('a' + p.material.color) << " gather=" << p.material.gather_cost
           << " detour=" << p.material.detour_cost << " value=" << p.value() << '\n';
    }
  }

  vector<MaterialCandidate> get_material_candidates(int px, int py) const {
    auto platform_dist = calc_distance_from(px, py);
    vector<MaterialCandidate> candidates;
    for (const auto &s : slime_info) {
      int gather_cost = platform_dist[s.initial_x][s.initial_y];
      int nest_cost = Dist[s.color][px][py];
      int original_cost = Dist[s.color][s.initial_x][s.initial_y];
      if (gather_cost == INF || nest_cost == INF || original_cost == INF) {
        continue;
      }
      int detour_cost = gather_cost + nest_cost - original_cost;
      candidates.push_back(MaterialCandidate{
          s.id,
          s.color,
          gather_cost,
          nest_cost,
          original_cost,
          detour_cost,
          detour_cost == 0,
      });
    }
    return candidates;
  }

  MaterialPlan evaluate_materials(int px, int py, int height) const {
    auto candidates = get_material_candidates(px, py);
    vector<vector<MaterialCandidate>> by_color(P.K);
    for (const auto &candidate : candidates) {
      by_color[candidate.color].push_back(candidate);
    }
    MaterialPlan best;
    for (int color = 0; color < P.K; ++color) {
      auto &vec = by_color[color];
      if (static_cast<int>(vec.size()) < height) {
        continue;
      }
      sort(vec.begin(), vec.end(), [](const MaterialCandidate &a, const MaterialCandidate &b) {
        if (a.detour_cost != b.detour_cost) {
          return a.detour_cost < b.detour_cost;
        }
        return a.gather_cost < b.gather_cost;
      });
      int detour_cost = 0;
      int gather_cost = 0;
      for (int i = 0; i < height; ++i) {
        detour_cost += vec[i].detour_cost;
        gather_cost += vec[i].gather_cost;
      }
      if (!best.valid() || detour_cost < best.detour_cost ||
          (detour_cost == best.detour_cost && gather_cost < best.gather_cost)) {
        best.color = color;
        best.detour_cost = detour_cost;
        best.gather_cost = gather_cost;
        best.slimes.assign(vec.begin(), vec.begin() + height);
      }
    }
    return best;
  }

  pair<int, int> find_slime(int id, const vector<vector<vector<int>>> &Slimes) const {
    for (int x = 0; x < P.N; ++x) {
      for (int y = 0; y < P.N; ++y) {
        for (int slime_id : Slimes[x][y]) {
          if (slime_id == id) {
            return {x, y};
          }
        }
      }
    }
    return {-1, -1};
  }

  bool move_material_to_platform(int id, int px, int py, vector<vector<vector<int>>> &Slimes,
                                 const vector<vector<int>> &Nests, Answer &ans) {
    auto [sx, sy] = find_slime(id, Slimes);
    // 既に帰巣しているなど、盤面上に存在しない
    if (sx == -1) {
      return false;
    }
    // 既にジャンプ台位置にいる
    if (sx == px && sy == py) {
      return true;
    }
    // 単純版では、材料スライムが単独で存在する場合だけ動かす
    if (Slimes[sx][sy].size() != 1) {
      return false;
    }
    // BFS
    vector dist(P.N, vector<int>(P.N, INF));
    vector prev_dir(P.N, vector<int>(P.N, -1));
    queue<pair<int, int>> que;
    dist[sx][sy] = 0;
    que.emplace(sx, sy);
    while (!que.empty()) {
      auto [x, y] = que.front();
      que.pop();
      if (x == px && y == py) {
        break;
      }
      for (int d = 0; d < 4; ++d) {
        int nx = x + dx[d];
        int ny = y + dy[d];
        if (!grid_check(nx, ny, P.N, P.N)) {
          continue;
        }
        if (P.C[nx][ny] == '#') {
          continue;
        }
        if (dist[nx][ny] != INF) {
          continue;
        }
        // ジャンプ台位置以外では、空マスだけを通る
        if (!(nx == px && ny == py) && !Slimes[nx][ny].empty()) {
          continue;
        }
        dist[nx][ny] = dist[x][y] + 1;
        prev_dir[nx][ny] = d;
        que.emplace(nx, ny);
      }
    }
    if (dist[px][py] == INF) {
      return false;
    }
    // 経路復元
    vector<int> path;
    int x = px;
    int y = py;
    while (x != sx || y != sy) {
      int d = prev_dir[x][y];
      path.push_back(d);
      x -= dx[d];
      y -= dy[d];
    }
    reverse(path.begin(), path.end());
    // 1マスずつ材料スライムを運ぶ
    x = sx;
    y = sy;
    for (int d : path) {
      int nx = x + dx[d];
      int ny = y + dy[d];
      Legal_action action{
          x, y, 0, d, 1,
      };
      ans.actions.push_back(action);
      apply_action(action, Slimes, Nests);
      // 材料が途中で帰巣してしまった場合
      if (nx != px || ny != py) {
        if (Slimes[nx][ny].empty() || Slimes[nx][ny].back() != id) {
          return false;
        }
      }
      x = nx;
      y = ny;
    }
    // 最後に材料がジャンプ台上に存在することを確認
    return find_slime(id, Slimes) == pair<int, int>{px, py};
  }

  bool build_jump_platform(const JumpPlatform &platform, vector<vector<vector<int>>> &Slimes,
                           const vector<vector<int>> &Nests, Answer &ans) {
    for (const auto &material : platform.material.slimes) {
      if (!move_material_to_platform(material.id, platform.x, platform.y, Slimes, Nests, ans)) {
        return false;
      }
    }
    return static_cast<int>(Slimes[platform.x][platform.y].size()) >= platform.height;
  }

  void initialize_field(vector<vector<vector<int>>> &Slimes, vector<vector<int>> &Nests) {
    for (int x = 0; x < P.N; ++x) {
      for (int y = 0; y < P.N; ++y) {
        char c = P.C[x][y];
        if (c == '#') {
          Nests[x][y] = -2;
        } else if ('A' <= c && c <= 'L') {
          Nests[x][y] = c - 'A';
        } else {
          Nests[x][y] = -1;
        }
      }
    }
    for (const auto &s : slime_info) {
      Slimes[s.initial_x][s.initial_y].push_back(s.id);
    }
  }

  void jump_slimes(int x, int y, int nx, int ny, int n, vector<vector<vector<int>>> &Slimes) {
    for (int i = 0; i < n; ++i) {
      Slimes[nx][ny].push_back(Slimes[x][y].back());
      Slimes[x][y].pop_back();
    }
  }

  void resolve_slime(int x, int y, int nx, int ny, vector<ResolvedSlime> &resolved_slime,
                     vector<vector<vector<int>>> &Slimes, const vector<vector<int>> &Nests) {
    int h = Slimes[x][y].size();
    int i = h - 1;
    while (i >= 0 && slime_color(Slimes[x][y][i]) == Nests[x][y]) {
      int id = Slimes[x][y][i];
      resolved_slime.push_back(ResolvedSlime{id, x, y});
      Slimes[x][y].pop_back();
      --i;
    }
    h = Slimes[nx][ny].size();
    i = h - 1;
    while (i >= 0 && slime_color(Slimes[nx][ny][i]) == Nests[nx][ny]) {
      int id = Slimes[nx][ny][i];
      resolved_slime.push_back(ResolvedSlime{id, nx, ny});
      Slimes[nx][ny].pop_back();
      --i;
    }
  }

  void unresolve_slime(vector<ResolvedSlime> &resolved_slime, vector<vector<vector<int>>> &Slimes) {
    reverse(resolved_slime.begin(), resolved_slime.end());
    for (const auto &s : resolved_slime) {
      Slimes[s.x][s.y].push_back(s.id);
    }
    resolved_slime.clear();
  }

  int apply_action(const Legal_action &action, vector<vector<vector<int>>> &Slimes, const vector<vector<int>> &Nests) {
    int res = 0;
    int x = action.x;
    int y = action.y;
    int nx = x + dx[action.d] * action.l;
    int ny = y + dy[action.d] * action.l;
    int h = Slimes[x][y].size();
    jump_slimes(x, y, nx, ny, h - action.k, Slimes);
    h = Slimes[x][y].size();
    int i = h - 1;
    while (i >= 0 && slime_color(Slimes[x][y][i]) == Nests[x][y]) {
      Slimes[x][y].pop_back();
      ++res;
      --i;
    }
    h = Slimes[nx][ny].size();
    i = h - 1;
    while (i >= 0 && slime_color(Slimes[nx][ny][i]) == Nests[nx][ny]) {
      Slimes[nx][ny].pop_back();
      ++res;
      --i;
    }
    return res;
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  Solver sol;
}