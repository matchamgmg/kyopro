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
  vector<vector<deque<int>>> Slimes; // 0 ~ 11: color(slime)
  vector<vector<int>> Nests;         // 0 ~ 11: color(nest), -1: floor, -2: wall
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

struct Slime {
  int color;
  int x;
  int y;
};

struct Solver {
  Problem P;
  mt19937 mt = mt19937{0};
  TimeKeeper tk = TimeKeeper{1900};

  void initialize_field(vector<vector<vector<int>>> &Slimes, vector<vector<int>> &Nests) {
    for (int i = 0; i < P.N; ++i) {
      for (int j = 0; j < P.N; ++j) {
        char &c = P.C[i][j];
        if (c == '.') {
          Nests[i][j] = -1;
        } else if (c == '#') {
          Nests[i][j] = -2;
        } else if (c >= 'a') {
          Slimes[i][j].push_back(c - 'a');
          Nests[i][j] = -1;
        } else {
          Nests[i][j] = (c - 'A');
        }
      }
    }
  }
  void calc_all_color_distance(vector<vector<vector<int>>> &Dist, const vector<vector<vector<int>>> &Slimes,
                               const vector<vector<int>> &Nests) {
    vector<pair<int, int>> nest(P.K);
    for (int i = 0; i < P.N; ++i) {
      for (int j = 0; j < P.N; ++j) {
        if (Nests[i][j] >= 0) {
          nest[Nests[i][j]] = {i, j};
        }
      }
    }
    queue<pair<int, int>> que;
    for (int i = 0; i < P.K; ++i) {
      auto p = nest[i];
      Dist[i][p.first][p.second] = 0;
      que.push(p);
      while (!que.empty()) {
        p = que.front();
        int pd = Dist[i][p.first][p.second];
        que.pop();
        for (int d = 0; d < 4; ++d) {
          int nx = p.first + dx[d];
          int ny = p.second + dy[d];
          if (!(0 <= nx && nx < P.N && 0 <= ny && ny < P.N)) {
            continue;
          }
          if (Nests[nx][ny] == -2) {
            continue;
          }
          if (Dist[i][nx][ny] != INF) {
            continue;
          }
          Dist[i][nx][ny] = pd + 1;
          que.emplace(nx, ny);
        }
      }
    }
  }

  int calc_slime_rest(int color, const vector<vector<vector<int>>> &Slimes) {
    int res = 0;
    for (int x = 0; x < P.N; ++x) {
      for (int y = 0; y < P.N; ++y) {
        for (int c : Slimes[x][y]) {
          res += (c == color);
        }
      }
    }
    return res;
  }

  int calc_rocal_rest(int x, int y, const vector<vector<vector<int>>> &Slimes, const vector<vector<int>> &Nests,
                      const vector<vector<vector<int>>> &Dist) {
    int res = 0;
    int col = Nests[x][y];
    int n = Slimes[x][y].size();
    int s = n - 1;
    while (s >= 0 && Slimes[x][y][s] == col) {
      res += Dist[col][x][y];
      res--;
      --s;
    }
    bool under = false;
    while (s >= 0) {
      int c = Slimes[x][y][s];
      res += Dist[c][x][y];
      under |= (c == col);
      --s;
    }
    res += under * 2;
    int bonus = 0;
    int last = -1;
    for (int c : Slimes[x][y]) {
      if (last != c) {
        last = c;
      } else {
        bonus++;
      }
    }
    return res - bonus;
  }

  void jump_slimes(int x, int y, int nx, int ny, int n, vector<vector<vector<int>>> &Slimes) {
    for (int i = 0; i < n; ++i) {
      Slimes[nx][ny].push_back(Slimes[x][y].back());
      Slimes[x][y].pop_back();
    }
  }

  void resolve_slime(int x, int y, int nx, int ny, vector<Slime> &resolved_slime, vector<vector<vector<int>>> &Slimes,
                     const vector<vector<int>> &Nests) {
    int h = Slimes[x][y].size();
    int i = h - 1;
    while (i >= 0 && Slimes[x][y][i] == Nests[x][y]) {
      resolved_slime.emplace_back(Slimes[x][y][i], x, y);
      Slimes[x][y].pop_back();
      i--;
    }
    h = Slimes[nx][ny].size();
    i = h - 1;
    while (i >= 0 && Slimes[nx][ny][i] == Nests[nx][ny]) {
      resolved_slime.emplace_back(Slimes[nx][ny][i], nx, ny);
      Slimes[nx][ny].pop_back();
      i--;
    }
  }

  void unresolve_slime(vector<Slime> &resolved_slime, vector<vector<vector<int>>> &Slimes) {
    std::reverse(resolved_slime.begin(), resolved_slime.end());
    for (Slime &s : resolved_slime) {
      Slimes[s.x][s.y].push_back(s.color);
    }
    resolved_slime.clear();
  }

  Legal_action pre_action(int color, int x, int y, vector<vector<vector<int>>> &Slimes,
                          const vector<vector<int>> &Nests, const vector<vector<vector<int>>> &Dist) {
    int pdist = Dist[color][x][y];
    int h = Slimes[x][y].size();
    int i = h - 1;
    int siz = 0;
    while (i >= 0 && Slimes[x][y][i] == color) {
      siz++;
      i--;
    }
    int min_k = h - siz;
    int best_dist = 1e9;
    int best_d = -1, best_l = -1;
    for (int d = 0; d < 4; ++d) {
      for (int l = 1; l <= min_k + 1; ++l) {
        int nx = x + dx[d] * l;
        int ny = y + dy[d] * l;
        if (!(0 <= nx && nx < P.N && 0 <= ny && ny < P.N)) {
          break;
        }
        if (Nests[nx][ny] == -2) {
          break;
        }
        int dist = Dist[color][nx][ny];
        if (pdist <= dist) {
          continue;
        }
        if (!Slimes[nx][ny].empty() && Slimes[nx][ny].back() == color && Slimes[nx][ny].size() + siz <= 8) {
          dist--;
        }
        if (dist <= best_dist) {
          best_dist = dist;
          best_d = d;
          best_l = l;
        }
      }
    }
    int nx = x + dx[best_d] * best_l;
    int ny = y + dy[best_d] * best_l;
    if (Slimes[nx][ny].size() + siz <= 8) {
      return Legal_action{x, y, min_k, best_d, best_l};
    } else if (Slimes[nx][ny].back() != color) {
      int k = h - (8 - Slimes[nx][ny].size());
      return Legal_action{x, y, k, best_d, best_l};
    } else {
      return pre_action(color, nx, ny, Slimes, Nests, Dist);
    }
  }

  Legal_action get_color_fixed_action(int color, vector<vector<vector<int>>> &Slimes, const vector<vector<int>> &Nests,
                                      const vector<vector<vector<int>>> &Dist) {
    vector<Slime> candidate;
    int fartherest = -1;
    int min_tower_size = 1e9;
    for (int x = 0; x < P.N; ++x) {
      for (int y = 0; y < P.N; ++y) {
        if (Slimes[x][y].empty()) {
          continue;
        }
        if (Slimes[x][y].back() != color) {
          continue;
        }
        if (fartherest < Dist[color][x][y]) {
          candidate.clear();
          candidate.emplace_back(color, x, y);
          fartherest = Dist[color][x][y];
          int siz = 0;
          int i = Slimes[x][y].size() - 1;
          while (i >= 0 && Slimes[x][y][i] == color) {
            ++siz;
            --i;
          }
          min_tower_size = siz;
        } else if (fartherest == Dist[color][x][y]) {
          int siz = 0;
          int i = Slimes[x][y].size() - 1;
          while (i >= 0 && Slimes[x][y][i] == color) {
            ++siz;
            --i;
          }
          if (min_tower_size == siz) {
            candidate.emplace_back(color, x, y);
          } else if (min_tower_size > siz) {
            candidate.clear();
            candidate.emplace_back(color, x, y);
            min_tower_size = siz;
          }
        }
      }
    }
    int r = mt() % (candidate.size());
    int x = candidate[r].x, y = candidate[r].y;
    int h = Slimes[x][y].size();
    int min_k = h - min_tower_size;
    int best_dist = 1e9;
    int best_d = -1, best_l = -1;
    for (int d = 0; d < 4; ++d) {
      for (int l = 1; l <= min_k + 1; ++l) {
        int nx = x + dx[d] * l;
        int ny = y + dy[d] * l;
        if (!(0 <= nx && nx < P.N && 0 <= ny && ny < P.N)) {
          break;
        }
        if (Nests[nx][ny] == -2) {
          break;
        }
        int dist = Dist[color][nx][ny];
        if (fartherest <= dist) {
          continue;
        }
        if (!Slimes[nx][ny].empty() && Slimes[nx][ny].back() == color && Slimes[nx][ny].size() + min_tower_size <= 8) {
          dist--;
        }
        if (dist <= best_dist) {
          best_dist = dist;
          best_d = d;
          best_l = l;
        }
      }
    }
    int nx = x + dx[best_d] * best_l;
    int ny = y + dy[best_d] * best_l;
    if (Slimes[nx][ny].size() + min_tower_size <= 8) {
      return Legal_action{x, y, min_k, best_d, best_l};
    } else if (Slimes[nx][ny].back() != color) {
      int k = h - (8 - Slimes[nx][ny].size());
      return Legal_action{x, y, k, best_d, best_l};
    } else {
      return pre_action(color, nx, ny, Slimes, Nests, Dist);
    }
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
    while (i >= 0 && Slimes[x][y][i] == Nests[x][y]) {
      Slimes[x][y].pop_back();
      res++;
      i--;
    }
    h = Slimes[nx][ny].size();
    i = h - 1;
    while (i >= 0 && Slimes[nx][ny][i] == Nests[nx][ny]) {
      Slimes[nx][ny].pop_back();
      res++;
      i--;
    }
    return res;
  }

  Answer solve_color_fixed() {
    Answer best_ans, ans;
    int best_score = 1e9, score;
    Legal_action action;
    vector<vector<vector<int>>> Slimes_original(P.N, vector<vector<int>>(P.N));
    vector<vector<vector<int>>> Slimes;
    vector<vector<int>> Nests(P.N, vector<int>(P.N));
    initialize_field(Slimes_original, Nests);
    vector<vector<vector<int>>> Dist(P.K, vector<vector<int>>(P.N, vector<int>(P.N, INF)));
    calc_all_color_distance(Dist, Slimes_original, Nests);
    // 色順ランダム
    vector<int> Colors(P.K);
    std::iota(Colors.begin(), Colors.end(), 0);
    set<vector<int>> st;
    int ri, rj;
    do {
      if (st.contains(Colors)) {
        continue;
      } else {
        st.insert(Colors);
      }
      ans.actions.clear();
      score = 0;
      Slimes = Slimes_original;
      for (int col : Colors) {
        int col_slimes = calc_slime_rest(col, Slimes);
        while (col_slimes > 0) {
          action = get_color_fixed_action(col, Slimes, Nests, Dist);
          col_slimes -= apply_action(action, Slimes, Nests);
          ans.actions.push_back(action);
          score++;
        }
      }
      if (best_score >= score) {
        best_score = score;
        std::swap(best_ans, ans);
      }
      ri = mt() % P.K;
      do {
        rj = mt() % P.K;
      } while (rj == ri);
      std::swap(Colors[ri], Colors[rj]);
    } while (!tk.isTimeOver());
    return best_ans;
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  Solver sol;
  Answer ans = sol.solve_color_fixed();
  ans.print();
}