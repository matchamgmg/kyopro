#include <bits/stdc++.h>
using namespace std;

constexpr int MAX_ACTIONS = 100000;
constexpr int TIME_LIMIT_MS = 1900;

// fill8 / flushX 用の局所探索。
// X = 7, 6, 5, 4 をすべて試し、完全解のうち最小手数を採用する。
constexpr int LOCAL_ROUTE_HORIZON = 24;
constexpr int LOCAL_SEARCH_DEPTH = 5;
constexpr int LOCAL_BEAM_WIDTH = 64;
constexpr int LOCAL_SETUP_CANDIDATE_LIMIT = 24;

// color-fixed の borrowed-helper 近傍。
// flush packet が他色の純粋towerへ着地したあと、helperを数手だけ同行させ、
// helperがtopに来たタイミングで自身のtreeへ切り離す。
// attach は既存の Flush move そのものなので追加コストなし。
constexpr int BORROW_MAX_SHARED_STEPS = 3;
constexpr int BORROW_DETACH_CANDIDATE_LIMIT = 8;
constexpr int BORROW_FUTURE_CREDIT_CAP = 3;

// 再利用可能なジャンプ台の将来価値。
// 現在のflushだけでなく、後続flushが同じ台を使える場合に
// 局所探索で少し余分なsetupコストを許容する。
constexpr int REUSE_CREDIT_PER_LEVEL = 1;

// color-mixed phase:
// 個数の少ない色だけを同時に扱い、最大4匹のtop segmentを
// 2色までのbundleとして輸送する。通常時は短いbeam searchで
// 合流準備から共通輸送までを見る。詰まった場合は一時的に
// 単色top blockを巣方向へ送るrecoveryへ切り替える。
constexpr int SMALL_COLOR_MAX_SLIMES = 7;
constexpr int MIXED_MAX_PACKET = 4;
constexpr int MIXED_MAX_COLORS = 2;
constexpr int MIXED_MAX_STEPS = 4000;

// small-color mixed transport 本体の短いbeam search。
// 1手貪欲では見えない
//   合流準備 -> bundle形成 -> 共通輸送
// を数手まとめて評価する。
constexpr int MIXED_SEARCH_DEPTH = 5;
constexpr int MIXED_SEARCH_BEAM_WIDTH = 64;

// mixed beam では、合流のために一時的に巣距離が1だけ悪化する手も許す。
constexpr int MIXED_MAX_BACKTRACK = 1;

// packet pair の meeting 候補は、両packetからこの距離以内のセルを調べる。
// beam は毎回張り直すので、遠いpairも近づけば後続探索で候補になる。
constexpr int MIXED_MEETING_RADIUS = 8;

// 1状態から全合法手を展開すると、+1 detour を許したことで枝数が急増する。
// meeting target への接近を局所scoreへ入れた上で上位だけをbeamへ渡す。
constexpr int MIXED_MOVE_CANDIDATE_LIMIT = 96;

constexpr int MIXED_RECOVERY_DEPTH = 5;
constexpr int MIXED_RECOVERY_BEAM_WIDTH = 32;

constexpr array<int, 4> DX = {-1, 1, 0, 0};
constexpr array<int, 4> DY = {0, 0, -1, 1};
constexpr array<char, 4> DIR_CHAR = {'U', 'D', 'L', 'R'};

struct Problem {
  int N, K;
  vector<string> C;

  Problem() {
    cin >> N >> K;
    C.resize(N);
    for (auto &row : C)
      cin >> row;
  }
};

struct Action {
  int x;
  int y;
  int k;
  int d;
  int l;

  friend ostream &operator<<(ostream &os, const Action &a) {
    return os << a.x << ' ' << a.y << ' ' << a.k << ' ' << DIR_CHAR[a.d] << ' ' << a.l;
  }
};

struct Tree {
  // adj[v] の bit d が立っていれば、d 方向に木の辺がある。
  vector<uint8_t> adj;

  // 巣を根とした情報。
  vector<int> parent;
  vector<int> depth;
  vector<int> child_count;
  vector<char> used;

  int edge_count = 0;
};

struct SimulationResult {
  bool success = false;
  vector<Action> actions;
};

struct MeetingPlan {
  bool valid = false;
  int meeting = -1;
  int shared_length = 0;
  int extra_detour = 0;
  int split_cost = 0;
  int estimated_saving = INT_MIN;
  int approach_distance = INT_MAX;
  int readiness = INT_MIN;
};

// 盤面は最大20x20、各towerは最大8段なので固定長で保持する。
// vector<vector<int>> と違い、盤面コピー時にセルごとのheap allocationが発生しない。
constexpr int MAX_CELLS = 20 * 20;

struct Tower {
  array<uint8_t, 8> slime{};
  uint8_t height = 0;

  bool empty() const {
    return height == 0;
  }
  int size() const {
    return (int)height;
  }
  int back() const {
    return (int)slime[height - 1];
  }
  int operator[](int i) const {
    return (int)slime[i];
  }

  void push_back(int color) {
    assert(height < 8);
    slime[height++] = (uint8_t)color;
  }

  void pop_back() {
    assert(height > 0);
    --height;
  }
};

using Board = array<Tower, MAX_CELLS>;
static_assert(is_trivially_copyable_v<Board>);

struct Solver {
  Problem P;
  int V;

  vector<int> nest;
  vector<int> nest_color;
  vector<vector<int>> terminals;
  vector<Tree> trees;

  // dist_to_nest[color][v]: v から color の巣までの最短距離。
  vector<vector<int>> dist_to_nest;

  // floor_dist[u][v]: 壁を避けた盤面上の全点対最短距離。
  // flush経路上判定とジャンプ台再利用価値の推定に使う。
  vector<vector<int>> floor_dist;

  // mixed_common_len[(a*K+b)][v]:
  // v から a,b の両方について最短路方向へ同時に進める最大連続長。
  // mixed packet の共有輸送区間の長さを近似する。
  vector<vector<int>> mixed_common_len;

  // meeting探索を軽くするため、各セルから一定半径内のセルだけ列挙しておく。
  vector<vector<int>> mixed_near_cells;

  // best meeting は盤面のtower高さではなく、色とpacket位置だけで評価する。
  // beam中に同じpairが何度も現れるのでキャッシュする。
  mutable unordered_map<uint64_t, MeetingPlan> mixed_meeting_plan_cache;

  // shortest_path_demand[color][v]:
  // 初期terminalのうち、v を通る最短路を持つものの個数。
  // flush size ごとの再利用価値は、この需要を packet size で割って評価する。
  vector<vector<int>> shortest_path_demand;

  // flushの最短路領域は盤面状態に依存しないため、
  // (active color, flush source) ごとに一度だけ構築して再利用する。
  vector<vector<vector<int>>> flush_route_cache;
  vector<vector<array<uint8_t, MAX_CELLS>>> flush_route_mask_cache;

  // 現在試している fill8 / flushX の X。
  // solve() から 7, 6, 5, 4 の順に切り替える。
  int strategy_flush_size = 4;

  // pure color-fixed と borrowed-helper 版を同じ色順で比較するための切替。
  bool borrow_neighbor_enabled = false;

  struct MixedMove {
    int source = -1;
    int destination = -1;
    int moved_size = 0;
    long long score = LLONG_MIN;
    int distance_gain = 0;
    int homed = 0;
  };

  struct MixedPhaseResult {
    bool success = false;
    Board board{};
    vector<Action> actions;
    int normal_moves = 0;
    int recovery_moves = 0;
    int recovery_calls = 0;
  };

  struct MixedSearchState {
    Board board{};
    array<MixedMove, MIXED_SEARCH_DEPTH> moves{};
    array<uint64_t, MIXED_SEARCH_DEPTH + 1> path_hash{};
    uint8_t move_count = 0;
    int remaining = INT_MAX;
    int distance_sum = INT_MAX;
    // 以降を各色別々に運ぶと仮定した推定操作数と、
    // 既存mixed bundle / 将来meetingで節約できる推定操作数。
    int independent_ops = INT_MAX;
    int bundle_value = 0;
    int meeting_value = 0;
    int meeting_readiness = 0;
    int estimated_remaining_ops = INT_MAX;
    long long score = LLONG_MIN;
    uint64_t hash = 0;
  };

  struct RecoveryState {
    Board board{};
    array<MixedMove, MIXED_RECOVERY_DEPTH> moves{};
    uint8_t move_count = 0;
    int remaining = INT_MAX;
    int distance_sum = INT_MAX;
    long long score = LLONG_MIN;
    uint64_t hash = 0;
  };

  mt19937 mt{0};
  chrono::steady_clock::time_point start_time;

  Solver() : V(P.N * P.N), start_time(chrono::steady_clock::now()) {
    nest.assign(P.K, -1);
    nest_color.assign(V, -1);
    terminals.resize(P.K);

    initialize_problem();
    build_distances();
    build_mixed_pair_cache();
    build_flush_route_cache();
    build_all_trees();
  }

  // =========================================================
  // Grid
  // =========================================================

  int cell_id(int x, int y) const {
    return x * P.N + y;
  }

  pair<int, int> cell_pos(int v) const {
    return {v / P.N, v % P.N};
  }

  bool inside(int x, int y) const {
    return 0 <= x && x < P.N && 0 <= y && y < P.N;
  }

  bool passable(int x, int y) const {
    return inside(x, y) && P.C[x][y] != '#';
  }

  int next_cell(int v, int d) const {
    auto [x, y] = cell_pos(v);
    int nx = x + DX[d];
    int ny = y + DY[d];
    if (!passable(nx, ny))
      return -1;
    return cell_id(nx, ny);
  }

  int direction_between(int u, int v) const {
    auto [ux, uy] = cell_pos(u);
    auto [vx, vy] = cell_pos(v);

    for (int d = 0; d < 4; ++d) {
      if (ux + DX[d] == vx && uy + DY[d] == vy)
        return d;
    }
    return -1;
  }

  bool time_over() const {
    auto now = chrono::steady_clock::now();
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(now - start_time).count();
    return elapsed >= TIME_LIMIT_MS;
  }

  // =========================================================
  // Input / initial information
  // =========================================================

  void initialize_problem() {
    for (int x = 0; x < P.N; ++x) {
      for (int y = 0; y < P.N; ++y) {
        int v = cell_id(x, y);
        char c = P.C[x][y];

        if ('A' <= c && c <= 'L') {
          int color = c - 'A';
          nest[color] = v;
          nest_color[v] = color;
        } else if ('a' <= c && c <= 'l') {
          int color = c - 'a';
          terminals[color].push_back(v);
        }
      }
    }
  }

  // =========================================================
  // Shortest path distances
  // =========================================================

  void build_distances() {
    dist_to_nest.assign(P.K, vector<int>(V, INT_MAX));

    for (int color = 0; color < P.K; ++color) {
      queue<int> que;
      int root = nest[color];
      dist_to_nest[color][root] = 0;
      que.push(root);

      while (!que.empty()) {
        int v = que.front();
        que.pop();

        for (int d = 0; d < 4; ++d) {
          int nv = next_cell(v, d);
          if (nv == -1)
            continue;
          if (dist_to_nest[color][nv] != INT_MAX)
            continue;

          dist_to_nest[color][nv] = dist_to_nest[color][v] + 1;
          que.push(nv);
        }
      }
    }

    floor_dist.assign(V, vector<int>(V, INT_MAX));

    for (int source = 0; source < V; ++source) {
      auto [sx, sy] = cell_pos(source);
      if (!passable(sx, sy))
        continue;

      queue<int> que;
      floor_dist[source][source] = 0;
      que.push(source);

      while (!que.empty()) {
        int v = que.front();
        que.pop();

        for (int d = 0; d < 4; ++d) {
          int nv = next_cell(v, d);
          if (nv == -1)
            continue;
          if (floor_dist[source][nv] != INT_MAX)
            continue;

          floor_dist[source][nv] = floor_dist[source][v] + 1;
          que.push(nv);
        }
      }
    }

    // 各セルが、その色の何匹の初期terminalから巣への最短路に
    // 含まれ得るかを数える。
    // flush size は戦略ごとに変わるので、ここでは需要そのものだけ保存する。
    shortest_path_demand.assign(P.K, vector<int>(V, 0));

    for (int color = 0; color < P.K; ++color) {
      for (int v = 0; v < V; ++v) {
        if (dist_to_nest[color][v] == INT_MAX)
          continue;

        int demand = 0;
        for (int u : terminals[color]) {
          if (floor_dist[u][v] == INT_MAX)
            continue;
          if (floor_dist[u][v] + dist_to_nest[color][v] == dist_to_nest[color][u]) {
            ++demand;
          }
        }

        shortest_path_demand[color][v] = demand;
      }
    }
  }

  void build_mixed_pair_cache() {
    mixed_common_len.assign(P.K * P.K, vector<int>(V, 0));

    // 共通最短路は、1歩進むたび両色の巣距離が1ずつ減るDAGになる。
    // 距離和の小さいセルからDPすれば、各セルからの最長共有区間が求まる。
    for (int a = 0; a < P.K; ++a) {
      for (int b = a + 1; b < P.K; ++b) {
        vector<int> order;
        order.reserve(V);
        for (int v = 0; v < V; ++v) {
          auto [x, y] = cell_pos(v);
          if (!passable(x, y))
            continue;
          if (dist_to_nest[a][v] == INT_MAX || dist_to_nest[b][v] == INT_MAX) {
            continue;
          }
          order.push_back(v);
        }

        sort(order.begin(), order.end(), [&](int lhs, int rhs) {
          int dl = dist_to_nest[a][lhs] + dist_to_nest[b][lhs];
          int dr = dist_to_nest[a][rhs] + dist_to_nest[b][rhs];
          if (dl != dr)
            return dl < dr;
          return lhs < rhs;
        });

        vector<int> dp(V, 0);
        for (int v : order) {
          int best = 0;
          for (int d = 0; d < 4; ++d) {
            int nv = next_cell(v, d);
            if (nv == -1)
              continue;
            if (!is_shortest_step(a, v, nv))
              continue;
            if (!is_shortest_step(b, v, nv))
              continue;
            best = max(best, 1 + dp[nv]);
          }
          dp[v] = best;
        }

        mixed_common_len[a * P.K + b] = dp;
        mixed_common_len[b * P.K + a] = dp;
      }
    }

    mixed_near_cells.assign(V, {});
    for (int u = 0; u < V; ++u) {
      auto [x, y] = cell_pos(u);
      if (!passable(x, y))
        continue;
      for (int v = 0; v < V; ++v) {
        if (floor_dist[u][v] <= MIXED_MEETING_RADIUS) {
          mixed_near_cells[u].push_back(v);
        }
      }
    }

    mixed_meeting_plan_cache.reserve(1 << 16);
  }

  bool is_shortest_step(int color, int from, int to) const {
    if (from < 0 || to < 0)
      return false;
    if (dist_to_nest[color][from] == INT_MAX)
      return false;
    return dist_to_nest[color][to] + 1 == dist_to_nest[color][from];
  }

  /**
   * source -> v -> nest が最短路になるか。
   */
  bool on_some_shortest_path(int color, int source, int v) const {
    if (source < 0 || v < 0)
      return false;
    if (floor_dist[source][v] == INT_MAX)
      return false;
    if (dist_to_nest[color][source] == INT_MAX || dist_to_nest[color][v] == INT_MAX) {
      return false;
    }

    return floor_dist[source][v] + dist_to_nest[color][v] == dist_to_nest[color][source];
  }

  // =========================================================
  // Greedy Steiner tree
  // =========================================================

  void add_tree_edge(Tree &tree, int u, int v) const {
    int d = direction_between(u, v);
    assert(d != -1);

    uint8_t bu = uint8_t(1u << d);
    uint8_t bv = uint8_t(1u << (d ^ 1));

    if (!(tree.adj[u] & bu))
      ++tree.edge_count;

    tree.adj[u] |= bu;
    tree.adj[v] |= bv;
  }

  bool has_tree_edge(const Tree &tree, int v, int d) const {
    return (tree.adj[v] & uint8_t(1u << d)) != 0;
  }

  /**
   * 巣だけから始め、現在の木全体を multi-source BFS の始点として、
   * 木に最も近い未接続スライムを1体ずつ接続する。
   *
   * これを各色について固定木として一度だけ構築する。
   */
  Tree build_greedy_tree(int color) const {
    Tree tree;
    tree.adj.assign(V, 0);

    vector<char> in_tree(V, false);
    vector<char> is_terminal(V, false);

    for (int v : terminals[color])
      is_terminal[v] = true;

    const int root = nest[color];
    in_tree[root] = true;

    while (true) {
      bool finished = true;
      for (int v : terminals[color]) {
        if (!in_tree[v]) {
          finished = false;
          break;
        }
      }
      if (finished)
        break;

      queue<int> que;
      vector<int> prev(V, -2);

      // 現在の木全体を始点にする。
      for (int v = 0; v < V; ++v) {
        if (!in_tree[v])
          continue;
        prev[v] = -1;
        que.push(v);
      }

      int target = -1;

      while (!que.empty()) {
        int v = que.front();
        que.pop();

        if (is_terminal[v] && !in_tree[v]) {
          target = v;
          break;
        }

        for (int d = 0; d < 4; ++d) {
          int nv = next_cell(v, d);
          if (nv == -1)
            continue;
          if (prev[nv] != -2)
            continue;

          prev[nv] = v;
          que.push(nv);
        }
      }

      assert(target != -1);

      int cur = target;
      while (prev[cur] != -1) {
        int p = prev[cur];
        add_tree_edge(tree, cur, p);
        in_tree[cur] = true;
        in_tree[p] = true;
        cur = p;
      }
    }

    return tree;
  }

  void build_root_info(int color, Tree &tree) const {
    tree.parent.assign(V, -2);
    tree.depth.assign(V, -1);
    tree.child_count.assign(V, 0);
    tree.used.assign(V, false);

    int root = nest[color];

    queue<int> que;
    tree.parent[root] = -1;
    tree.depth[root] = 0;
    tree.used[root] = true;
    que.push(root);

    while (!que.empty()) {
      int v = que.front();
      que.pop();

      for (int d = 0; d < 4; ++d) {
        if (!has_tree_edge(tree, v, d))
          continue;

        int nv = next_cell(v, d);
        assert(nv != -1);

        if (tree.parent[nv] != -2)
          continue;

        tree.parent[nv] = v;
        tree.depth[nv] = tree.depth[v] + 1;
        tree.used[nv] = true;
        ++tree.child_count[v];
        que.push(nv);
      }
    }
  }

  void build_all_trees() {
    trees.resize(P.K);

    for (int color = 0; color < P.K; ++color) {
      trees[color] = build_greedy_tree(color);
      build_root_info(color, trees[color]);
    }
  }

  // =========================================================
  // Simulation state helpers
  // =========================================================

  Board initial_board() const {
    Board board{};

    for (int color = 0; color < P.K; ++color) {
      for (int v : terminals[color]) {
        board[v].push_back(color);
      }
    }

    return board;
  }

  int top_color(const Board &board, int v) const {
    if (board[v].empty())
      return -1;
    return board[v].back();
  }

  int top_block_size(const Board &board, int v) const {
    if (board[v].empty())
      return 0;

    int color = board[v].back();
    int cnt = 0;

    for (int i = (int)board[v].size() - 1; i >= 0 && board[v][i] == color; --i) {
      ++cnt;
    }

    return cnt;
  }

  int count_color(const Board &board, int color) const {
    int result = 0;
    for (int v = 0; v < V; ++v) {
      const auto &tower = board[v];
      for (int i = 0; i < tower.size(); ++i) {
        result += (tower[i] == color);
      }
    }
    return result;
  }

  int count_all(const Board &board) const {
    int result = 0;
    for (int v = 0; v < V; ++v)
      result += board[v].size();
    return result;
  }

  void home_cell(Board &board, int v) const {
    int color = nest_color[v];
    if (color == -1)
      return;

    while (!board[v].empty() && board[v].back() == color) {
      board[v].pop_back();
    }
  }

  // =========================================================
  // Actions
  // =========================================================

  bool execute_action(Board &board, vector<Action> &actions, int source, int moved_size, int destination) const {
    if ((int)actions.size() >= MAX_ACTIONS)
      return false;
    if (board[source].empty())
      return false;
    if (!(1 <= moved_size && moved_size <= (int)board[source].size()))
      return false;
    if ((int)board[destination].size() + moved_size > 8)
      return false;

    auto [sx, sy] = cell_pos(source);
    auto [tx, ty] = cell_pos(destination);

    int d = -1;
    int l = -1;

    if (sx == tx) {
      if (sy < ty) {
        d = 3;
        l = ty - sy;
      } else if (ty < sy) {
        d = 2;
        l = sy - ty;
      }
    } else if (sy == ty) {
      if (sx < tx) {
        d = 1;
        l = tx - sx;
      } else if (tx < sx) {
        d = 0;
        l = sx - tx;
      }
    }

    if (d == -1 || l <= 0)
      return false;

    int h = board[source].size();
    int k = h - moved_size;

    if (!(0 <= k && k < h))
      return false;
    if (l > k + 1)
      return false;

    // 通過マスはすべて床でなければならない。
    int v = source;
    for (int step = 0; step < l; ++step) {
      v = next_cell(v, d);
      if (v == -1)
        return false;
    }
    if (v != destination)
      return false;

    actions.push_back({sx, sy, k, d, l});

    // 上から取り出すので移動部分は反転する。
    // 今回は同色blockのみを動かすため、色列としては変化しない。
    for (int i = 0; i < moved_size; ++i) {
      int color = board[source].back();
      board[source].pop_back();
      board[destination].push_back(color);
    }

    home_cell(board, source);
    home_cell(board, destination);

    return true;
  }

  // =========================================================
  // Small-color mixed transport
  // =========================================================

  bool is_small_color(int color) const {
    return 0 <= color && color < P.K && !terminals[color].empty() &&
           (int)terminals[color].size() <= SMALL_COLOR_MAX_SLIMES;
  }

  int count_small_slimes(const Board &board) const {
    int result = 0;
    for (int v = 0; v < V; ++v) {
      for (int i = 0; i < board[v].size(); ++i) {
        result += is_small_color(board[v][i]);
      }
    }
    return result;
  }

  int small_distance_sum(const Board &board) const {
    int result = 0;
    for (int v = 0; v < V; ++v) {
      for (int i = 0; i < board[v].size(); ++i) {
        int color = board[v][i];
        if (!is_small_color(color))
          continue;
        if (dist_to_nest[color][v] == INT_MAX)
          return INT_MAX / 4;
        result += dist_to_nest[color][v];
      }
    }
    return result;
  }

  int tower_small_count(const Tower &tower) const {
    int result = 0;
    for (int i = 0; i < tower.size(); ++i) {
      result += is_small_color(tower[i]);
    }
    return result;
  }

  int tower_small_distance(const Tower &tower, int v) const {
    int result = 0;
    for (int i = 0; i < tower.size(); ++i) {
      int color = tower[i];
      if (!is_small_color(color))
        continue;
      result += dist_to_nest[color][v];
    }
    return result;
  }

  int top_small_suffix_size(const Tower &tower) const {
    int result = 0;
    for (int i = tower.size() - 1; i >= 0; --i) {
      if (!is_small_color(tower[i]))
        break;
      ++result;
    }
    return result;
  }

  uint16_t top_small_color_mask(const Tower &tower) const {
    uint16_t mask = 0;
    for (int i = tower.size() - 1; i >= 0; --i) {
      int color = tower[i];
      if (!is_small_color(color))
        break;
      mask |= uint16_t(1u << color);
    }
    return mask;
  }

  int top_small_transitions(const Tower &tower) const {
    int suffix = top_small_suffix_size(tower);
    if (suffix <= 1)
      return 0;

    int first = tower.size() - suffix;
    int result = 0;
    for (int i = first + 1; i < tower.size(); ++i) {
      result += tower[i] != tower[i - 1];
    }
    return result;
  }

  void home_tower(Tower &tower, int v) const {
    int color = nest_color[v];
    if (color == -1)
      return;
    while (!tower.empty() && tower.back() == color)
      tower.pop_back();
  }

  void apply_move_unchecked(Board &board, int source, int moved_size, int destination) const {
    for (int i = 0; i < moved_size; ++i) {
      int color = board[source].back();
      board[source].pop_back();
      board[destination].push_back(color);
    }
    home_cell(board, source);
    home_cell(board, destination);
  }

  uint64_t hash_board(const Board &board) const {
    uint64_t h = 1469598103934665603ULL;
    auto mix = [&](uint64_t x) {
      h ^= x + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
      h *= 1099511628211ULL;
    };

    for (int v = 0; v < V; ++v) {
      if (board[v].empty())
        continue;
      mix((uint64_t)(v + 1));
      mix((uint64_t)board[v].size());
      for (int i = 0; i < board[v].size(); ++i) {
        mix((uint64_t)(board[v][i] + 1));
      }
    }
    return h;
  }

  int common_shortest_direction_count(uint16_t color_mask, int v) const {
    if (color_mask == 0)
      return 0;
    int result = 0;

    for (int d = 0; d < 4; ++d) {
      int nv = next_cell(v, d);
      if (nv == -1)
        continue;

      bool ok = true;
      for (int color = 0; color < P.K; ++color) {
        if (!(color_mask & uint16_t(1u << color)))
          continue;
        if (!is_shortest_step(color, v, nv)) {
          ok = false;
          break;
        }
      }
      result += ok;
    }

    return result;
  }

  int popcount_colors(uint16_t mask) const {
    return __builtin_popcount((unsigned)mask);
  }

  uint16_t top_packet_color_mask(const Tower &tower, int packet_size) const {
    uint16_t mask = 0;
    packet_size = min(packet_size, tower.size());
    for (int i = 0; i < packet_size; ++i) {
      int color = tower[tower.size() - 1 - i];
      if (!is_small_color(color))
        return 0;
      mask |= uint16_t(1u << color);
    }
    return mask;
  }

  int top_packet_transitions(const Tower &tower, int packet_size) const {
    packet_size = min(packet_size, tower.size());
    if (packet_size <= 1)
      return 0;
    int first = tower.size() - packet_size;
    int result = 0;
    for (int i = first + 1; i < tower.size(); ++i) {
      result += tower[i] != tower[i - 1];
    }
    return result;
  }

  int common_shortest_length(int a, int b, int v) const {
    if (a == b || a < 0 || b < 0 || v < 0)
      return 0;
    return mixed_common_len[a * P.K + b][v];
  }

  uint64_t meeting_plan_key(int color1, int pos1, int color2, int pos2) const {
    if (color1 > color2) {
      swap(color1, color2);
      swap(pos1, pos2);
    }
    // color < 16, position < 512 なので固定bitで十分。
    return (uint64_t)color1 | ((uint64_t)color2 << 4) | ((uint64_t)pos1 << 8) | ((uint64_t)pos2 << 17);
  }

  /**
   * pure packet 2個について最良meetingセルを求める。
   *
   * 別々に運ぶ推定コスト:
   *   dist(c1,p1) + dist(c2,p2)
   * meeting m で合流し、L手だけ共通最短路を運ぶ推定コスト:
   *   d(p1,m) + d(p2,m) + L
   *   + (dist(c1,m)-L) + (dist(c2,m)-L) + split_cost
   *
   * 差を整理すると
   *   saving = L - extra_detour1 - extra_detour2 - split_cost
   * となる。これを「予想削減操作数」として直接使う。
   */
  MeetingPlan best_meeting_plan(int color1, int pos1, int color2, int pos2) const {
    if (color1 == color2)
      return {};

    uint64_t key = meeting_plan_key(color1, pos1, color2, pos2);
    if (auto it = mixed_meeting_plan_cache.find(key); it != mixed_meeting_plan_cache.end()) {
      return it->second;
    }

    int a = color1, b = color2;
    int u = pos1, v = pos2;
    if (a > b) {
      swap(a, b);
      swap(u, v);
    }

    MeetingPlan best;

    if (floor_dist[u][v] == INT_MAX || floor_dist[u][v] > 2 * MIXED_MEETING_RADIUS) {
      mixed_meeting_plan_cache.emplace(key, best);
      return best;
    }

    for (int m : mixed_near_cells[u]) {
      int d1 = floor_dist[u][m];
      int d2 = floor_dist[v][m];
      if (d2 == INT_MAX || d2 > MIXED_MEETING_RADIUS)
        continue;
      if (dist_to_nest[a][m] == INT_MAX || dist_to_nest[b][m] == INT_MAX) {
        continue;
      }

      int shared = common_shortest_length(a, b, m);
      if (shared <= 0)
        continue;

      int extra1 = d1 + dist_to_nest[a][m] - dist_to_nest[a][u];
      int extra2 = d2 + dist_to_nest[b][m] - dist_to_nest[b][v];
      if (extra1 < 0 || extra2 < 0)
        continue;

      // 共有区間が近い方の巣まで到達できれば、巣で自然分離できる可能性が高い。
      int split_cost = (shared == min(dist_to_nest[a][m], dist_to_nest[b][m])) ? 0 : 1;

      int saving = shared - extra1 - extra2 - split_cost;
      int approach = d1 + d2;
      int readiness = 12 * shared - 2 * approach - 6 * (extra1 + extra2) - 4 * split_cost;

      bool better =
          !best.valid || saving > best.estimated_saving ||
          (saving == best.estimated_saving && readiness > best.readiness) ||
          (saving == best.estimated_saving && readiness == best.readiness && approach < best.approach_distance) ||
          (saving == best.estimated_saving && readiness == best.readiness && approach == best.approach_distance &&
           m < best.meeting);
      if (!better)
        continue;

      best.valid = true;
      best.meeting = m;
      best.shared_length = shared;
      best.extra_detour = extra1 + extra2;
      best.split_cost = split_cost;
      best.estimated_saving = saving;
      best.approach_distance = approach;
      best.readiness = readiness;
    }

    mixed_meeting_plan_cache.emplace(key, best);
    return best;
  }

  struct PurePacket {
    int v = -1;
    int color = -1;
    int size = 0;
  };

  vector<PurePacket> visible_pure_packets(const Board &board) const {
    vector<PurePacket> packets;
    packets.reserve(64);

    for (int v = 0; v < V; ++v) {
      if (board[v].empty())
        continue;
      int color = top_color(board, v);
      if (!is_small_color(color))
        continue;

      // すでに2色bundleになっているtowerは meeting 候補ではなく
      // existing bundle として別評価する。
      if (popcount_colors(top_small_color_mask(board[v])) != 1)
        continue;

      int block = top_block_size(board, v);
      int packet = min(MIXED_MAX_PACKET, block);
      if (packet <= 0)
        continue;
      packets.push_back({v, color, packet});
    }

    return packets;
  }

  struct MeetingEstimate {
    int saving = 0;
    int readiness = 0;
  };

  MeetingEstimate estimate_future_meeting(const Board &board, array<int, MAX_CELLS> *target_meeting = nullptr) const {
    if (target_meeting)
      target_meeting->fill(-1);

    auto packets = visible_pure_packets(board);

    struct Candidate {
      int i;
      int j;
      MeetingPlan plan;
    };
    vector<Candidate> candidates;
    candidates.reserve(packets.size() * 2);

    // sourceごとのtargetは、最もsavingが大きく、同値なら近いmeetingを使う。
    array<int, MAX_CELLS> target_quality{};
    target_quality.fill(INT_MIN);

    for (int i = 0; i < (int)packets.size(); ++i) {
      for (int j = i + 1; j < (int)packets.size(); ++j) {
        const auto &a = packets[i];
        const auto &b = packets[j];
        if (a.color == b.color)
          continue;
        if (a.size + b.size > MIXED_MAX_PACKET)
          continue;
        if (floor_dist[a.v][b.v] == INT_MAX || floor_dist[a.v][b.v] > 2 * MIXED_MEETING_RADIUS) {
          continue;
        }

        MeetingPlan plan = best_meeting_plan(a.color, a.v, b.color, b.v);
        if (!plan.valid || plan.estimated_saving <= 0)
          continue;

        candidates.push_back({i, j, plan});

        if (target_meeting) {
          int qa = 100 * plan.estimated_saving + plan.readiness - floor_dist[a.v][plan.meeting];
          int qb = 100 * plan.estimated_saving + plan.readiness - floor_dist[b.v][plan.meeting];
          if (qa > target_quality[a.v]) {
            target_quality[a.v] = qa;
            (*target_meeting)[a.v] = plan.meeting;
          }
          if (qb > target_quality[b.v]) {
            target_quality[b.v] = qb;
            (*target_meeting)[b.v] = plan.meeting;
          }
        }
      }
    }

    sort(candidates.begin(), candidates.end(), [](const Candidate &lhs, const Candidate &rhs) {
      if (lhs.plan.estimated_saving != rhs.plan.estimated_saving) {
        return lhs.plan.estimated_saving > rhs.plan.estimated_saving;
      }
      if (lhs.plan.readiness != rhs.plan.readiness) {
        return lhs.plan.readiness > rhs.plan.readiness;
      }
      return lhs.plan.approach_distance < rhs.plan.approach_distance;
    });

    // 同じpacketを複数の将来bundleへ二重計上しないよう、
    // 高価値pairからgreedy matchingする。
    vector<char> used(packets.size(), false);
    MeetingEstimate result;
    for (const auto &candidate : candidates) {
      if (used[candidate.i] || used[candidate.j])
        continue;
      used[candidate.i] = used[candidate.j] = true;
      result.saving += candidate.plan.estimated_saving;
      result.readiness += candidate.plan.readiness;
    }
    return result;
  }

  /**
   * small color を今後すべて「色blockごとに別々に」運ぶと仮定した推定操作数。
   * 同色の連続blockは最大4匹まで1packetとして同時に運べるため、
   * slime数ではなくpacket数 × 巣距離で数える。
   */
  int estimate_independent_small_ops(const Board &board) const {
    int result = 0;
    for (int v = 0; v < V; ++v) {
      const auto &tower = board[v];
      int i = 0;
      while (i < tower.size()) {
        int color = tower[i];
        if (!is_small_color(color)) {
          ++i;
          continue;
        }

        int j = i + 1;
        while (j < tower.size() && tower[j] == color)
          ++j;
        int block_size = j - i;
        int packet_count = (block_size + MIXED_MAX_PACKET - 1) / MIXED_MAX_PACKET;
        if (dist_to_nest[color][v] != INT_MAX) {
          result += packet_count * dist_to_nest[color][v];
        }
        i = j;
      }
    }
    return result;
  }

  /**
   * すでに出来ている2色mixed packetが、今後の共有最短路で
   * 別々に運ぶ場合より何操作節約できそうかを推定する。
   */
  int estimate_existing_bundle_saving(const Board &board) const {
    int saving = 0;

    for (int v = 0; v < V; ++v) {
      int suffix = top_small_suffix_size(board[v]);
      if (suffix <= 1)
        continue;

      int packet = min(MIXED_MAX_PACKET, suffix);
      uint16_t mask = top_packet_color_mask(board[v], packet);
      if (popcount_colors(mask) != 2)
        continue;

      int colors[2] = {-1, -1};
      int ptr = 0;
      for (int color = 0; color < P.K; ++color) {
        if (mask & uint16_t(1u << color))
          colors[ptr++] = color;
      }

      int shared = common_shortest_length(colors[0], colors[1], v);
      if (shared <= 0)
        continue;

      int split_cost = (shared == min(dist_to_nest[colors[0]][v], dist_to_nest[colors[1]][v])) ? 0 : 1;
      int transitions = top_packet_transitions(board[v], packet);

      // 交互積みは安全な分離が難しいので、その分だけ節約見積りを削る。
      int local = shared - split_cost - max(0, transitions - 1);
      saving += max(0, local);
    }

    return saving;
  }

  void update_mixed_search_features(MixedSearchState &state) const {
    state.independent_ops = estimate_independent_small_ops(state.board);
    state.bundle_value = estimate_existing_bundle_saving(state.board);
    auto meeting = estimate_future_meeting(state.board);
    state.meeting_value = meeting.saving;
    state.meeting_readiness = meeting.readiness;

    state.estimated_remaining_ops = max(0, state.independent_ops - state.bundle_value - state.meeting_value);
  }

  /**
   * mixed move の局所評価。
   *
   * 通常beamでは各色の巣距離を1だけ悪化させる一時detourを許す。
   * これにより「一度横へ寄ってmeeting -> その後bundleで共有輸送」が探索可能になる。
   */
  optional<MixedMove> evaluate_mixed_move(const Board &board, int source, int moved_size, int destination,
                                          int length) const {
    if (source < 0 || destination < 0 || source == destination)
      return nullopt;
    if (board[source].empty())
      return nullopt;
    if (!(1 <= moved_size && moved_size <= board[source].size()))
      return nullopt;
    if (board[destination].size() + moved_size > 8)
      return nullopt;

    uint16_t moved_mask = 0;
    int backtrack = 0;
    for (int i = 0; i < moved_size; ++i) {
      int color = board[source][board[source].size() - 1 - i];
      if (!is_small_color(color))
        return nullopt;
      moved_mask |= uint16_t(1u << color);

      int delta = dist_to_nest[color][destination] - dist_to_nest[color][source];
      if (delta > MIXED_MAX_BACKTRACK)
        return nullopt;
      backtrack += max(0, delta);
    }
    if (popcount_colors(moved_mask) > MIXED_MAX_COLORS)
      return nullopt;

    int before_count = tower_small_count(board[source]) + tower_small_count(board[destination]);
    int before_dist =
        tower_small_distance(board[source], source) + tower_small_distance(board[destination], destination);
    int destination_small_before = top_small_suffix_size(board[destination]);

    Tower source_after = board[source];
    Tower destination_after = board[destination];

    for (int i = 0; i < moved_size; ++i) {
      int color = source_after.back();
      source_after.pop_back();
      destination_after.push_back(color);
    }
    home_tower(source_after, source);
    home_tower(destination_after, destination);

    int after_count = tower_small_count(source_after) + tower_small_count(destination_after);
    int after_dist = tower_small_distance(source_after, source) + tower_small_distance(destination_after, destination);

    int homed = before_count - after_count;
    int distance_gain = before_dist - after_dist;

    int top_packet = min(MIXED_MAX_PACKET, top_small_suffix_size(destination_after));
    uint16_t result_mask = top_packet_color_mask(destination_after, top_packet);
    int result_colors = popcount_colors(result_mask);
    int transitions = top_packet_transitions(destination_after, top_packet);

    if (result_colors > MIXED_MAX_COLORS)
      return nullopt;
    if (transitions > 2)
      return nullopt;

    int common_dirs = common_shortest_direction_count(result_mask, destination);
    int jump_power = 0;
    if (top_packet > 0) {
      jump_power = destination_after.size() - top_packet + 1;
    }

    int merge_bonus = 0;
    if (destination_small_before > 0 && result_colors >= 1) {
      merge_bonus = min(destination_small_before, moved_size);
    }

    int split_bonus = 0;
    if (homed > 0 && top_small_suffix_size(destination_after) > 0) {
      split_bonus = 1;
    }

    int clean_bundle_bonus = 0;
    if (result_colors == 2) {
      clean_bundle_bonus = (transitions == 1 ? 2 : 0);
    }

    // これは候補の事前順位付けだけに使う。
    // beam本体の評価は「予想残り操作数」で行うため、距離gainを以前より弱くする。
    long long score = 0;
    score += 4000LL * homed;
    score += 60LL * distance_gain;
    score += 20LL * moved_size;
    score += 45LL * common_dirs * max(1, top_packet);
    score += 20LL * jump_power;
    score += 90LL * merge_bonus;
    score += 70LL * split_bonus;
    score += 70LL * clean_bundle_bonus;
    score -= 90LL * backtrack;

    if (result_colors == 2 && common_dirs == 0)
      score -= 80;
    score += 2LL * length;

    return MixedMove{
        source, destination, moved_size, score, distance_gain, homed,
    };
  }

  vector<MixedMove> generate_mixed_moves(const Board &board,
                                         const array<int, MAX_CELLS> *target_meeting = nullptr) const {
    vector<MixedMove> result;

    for (int source = 0; source < V; ++source) {
      int suffix = top_small_suffix_size(board[source]);
      if (suffix <= 0)
        continue;

      int max_packet = min(MIXED_MAX_PACKET, suffix);
      for (int moved_size = 1; moved_size <= max_packet; ++moved_size) {
        uint16_t moved_mask = 0;
        bool valid_packet = true;
        for (int i = 0; i < moved_size; ++i) {
          int color = board[source][board[source].size() - 1 - i];
          if (!is_small_color(color)) {
            valid_packet = false;
            break;
          }
          moved_mask |= uint16_t(1u << color);
        }
        if (!valid_packet || popcount_colors(moved_mask) > MIXED_MAX_COLORS) {
          continue;
        }

        int k = board[source].size() - moved_size;
        int max_jump = k + 1;

        for (int d = 0; d < 4; ++d) {
          int v = source;
          for (int l = 1; l <= max_jump; ++l) {
            int nv = next_cell(v, d);
            if (nv == -1)
              break;
            v = nv;

            if (board[v].size() + moved_size > 8)
              continue;

            auto candidate = evaluate_mixed_move(board, source, moved_size, v, l);
            if (!candidate.has_value())
              continue;

            // pairごとのbest meetingへ近づく手は、たとえ巣距離が+1でも
            // candidate prefilterで落ちにくくする。
            if (target_meeting && (*target_meeting)[source] != -1) {
              int target = (*target_meeting)[source];
              int before = floor_dist[source][target];
              int after = floor_dist[v][target];
              if (before != INT_MAX && after != INT_MAX) {
                candidate->score += 180LL * (before - after);
              }
            }

            result.push_back(*candidate);
          }
        }
      }
    }

    sort(result.begin(), result.end(), [](const MixedMove &a, const MixedMove &b) {
      if (a.score != b.score)
        return a.score > b.score;
      if (a.homed != b.homed)
        return a.homed > b.homed;
      if (a.distance_gain != b.distance_gain) {
        return a.distance_gain > b.distance_gain;
      }
      if (a.moved_size != b.moved_size)
        return a.moved_size > b.moved_size;
      if (a.source != b.source)
        return a.source < b.source;
      return a.destination < b.destination;
    });

    if ((int)result.size() > MIXED_MOVE_CANDIDATE_LIMIT) {
      result.resize(MIXED_MOVE_CANDIDATE_LIMIT);
    }
    return result;
  }

  long long mixed_search_score(const MixedSearchState &state) const {
    // 最優先は「ここまで使った手数 + 予想残り手数」。
    // 1操作の差を、下のtie-break heuristicより十分大きくする。
    long long predicted_total = (long long)state.move_count + state.estimated_remaining_ops;
    long long score = -10000LL * predicted_total;

    // 同じ予想操作数なら、meetingが実現に近い状態を残す。
    score += 20LL * state.meeting_readiness;
    score += 40LL * state.bundle_value;

    // さらに同程度なら、実際に帰巣が進み、総距離も短い状態を優先。
    score -= 8LL * state.remaining;
    score -= state.distance_sum;
    return score;
  }

  /**
   * small-color mixed transport のbeam search。
   *
   * 1. 巣距離+1までの一時detourを許す。
   * 2. pure packet pairごとにbest meeting cellを求め、そのmeetingへの接近を誘導する。
   * 3. 状態評価は距離総和ではなく、
   *      実手数 + (別々に運ぶ推定手数 - bundle/meetingの予想節約手数)
   *    を主指標にする。
   */
  vector<MixedMove> search_mixed_beam(const Board &start_board, const unordered_set<uint64_t> &global_visited) const {
    MixedSearchState initial;
    initial.board = start_board;
    initial.remaining = count_small_slimes(start_board);
    initial.distance_sum = small_distance_sum(start_board);
    initial.hash = hash_board(start_board);
    initial.path_hash[0] = initial.hash;
    update_mixed_search_features(initial);
    initial.score = mixed_search_score(initial);

    vector<MixedSearchState> beam{initial};
    optional<MixedSearchState> best;

    for (int depth = 0; depth < MIXED_SEARCH_DEPTH; ++depth) {
      vector<MixedSearchState> next;
      unordered_set<uint64_t> seen;
      seen.reserve(MIXED_SEARCH_BEAM_WIDTH * MIXED_MOVE_CANDIDATE_LIMIT);

      for (const auto &state : beam) {
        array<int, MAX_CELLS> meeting_target;
        // 親状態のpair planを使い、必要なdetour moveをcandidate上位へ持ち上げる。
        estimate_future_meeting(state.board, &meeting_target);
        auto moves = generate_mixed_moves(state.board, &meeting_target);

        for (const auto &move : moves) {
          MixedSearchState child = state;
          apply_move_unchecked(child.board, move.source, move.moved_size, move.destination);

          child.moves[child.move_count] = move;
          ++child.move_count;
          child.remaining = state.remaining - move.homed;
          child.distance_sum = state.distance_sum - move.distance_gain;
          child.hash = hash_board(child.board);

          if (global_visited.contains(child.hash))
            continue;

          bool loop = false;
          for (int i = 0; i < child.move_count; ++i) {
            if (child.path_hash[i] == child.hash) {
              loop = true;
              break;
            }
          }
          if (loop)
            continue;

          if (!seen.insert(child.hash).second)
            continue;
          child.path_hash[child.move_count] = child.hash;

          update_mixed_search_features(child);
          child.score = mixed_search_score(child);

          if (!best.has_value() || child.score > best->score ||
              (child.score == best->score && child.estimated_remaining_ops < best->estimated_remaining_ops) ||
              (child.score == best->score && child.estimated_remaining_ops == best->estimated_remaining_ops &&
               child.remaining < best->remaining) ||
              (child.score == best->score && child.estimated_remaining_ops == best->estimated_remaining_ops &&
               child.remaining == best->remaining && child.distance_sum < best->distance_sum)) {
            best = child;
          }

          if (child.remaining == 0) {
            vector<MixedMove> result;
            result.reserve(child.move_count);
            for (int i = 0; i < child.move_count; ++i) {
              result.push_back(child.moves[i]);
            }
            return result;
          }

          next.push_back(std::move(child));
        }
      }

      if (next.empty())
        break;

      sort(next.begin(), next.end(), [](const MixedSearchState &a, const MixedSearchState &b) {
        if (a.score != b.score)
          return a.score > b.score;
        if (a.estimated_remaining_ops != b.estimated_remaining_ops) {
          return a.estimated_remaining_ops < b.estimated_remaining_ops;
        }
        if (a.meeting_value != b.meeting_value) {
          return a.meeting_value > b.meeting_value;
        }
        if (a.meeting_readiness != b.meeting_readiness) {
          return a.meeting_readiness > b.meeting_readiness;
        }
        if (a.remaining != b.remaining)
          return a.remaining < b.remaining;
        if (a.distance_sum != b.distance_sum) {
          return a.distance_sum < b.distance_sum;
        }
        return a.hash < b.hash;
      });

      if ((int)next.size() > MIXED_SEARCH_BEAM_WIDTH) {
        next.resize(MIXED_SEARCH_BEAM_WIDTH);
      }
      beam = std::move(next);
    }

    if (!best.has_value())
      return {};

    vector<MixedMove> result;
    result.reserve(best->move_count);
    for (int i = 0; i < best->move_count; ++i) {
      result.push_back(best->moves[i]);
    }
    return result;
  }

  /**
   * recovery用候補。
   * topの単色blockだけを、その色自身の巣への最短路方向へ進める。
   * 通常のmixed評価を一時的に無視し、盤面の可動性と単調な距離進捗を回復する。
   */
  vector<MixedMove> generate_recovery_moves(const Board &board) const {
    vector<MixedMove> result;

    for (int source = 0; source < V; ++source) {
      if (board[source].empty())
        continue;
      int color = top_color(board, source);
      if (!is_small_color(color))
        continue;

      int block_size = top_block_size(board, source);
      int max_packet = min(MIXED_MAX_PACKET, block_size);

      for (int moved_size = 1; moved_size <= max_packet; ++moved_size) {
        int k = board[source].size() - moved_size;
        int max_jump = k + 1;

        for (int d = 0; d < 4; ++d) {
          int v = source;
          for (int l = 1; l <= max_jump; ++l) {
            int nv = next_cell(v, d);
            if (nv == -1)
              break;
            if (!is_shortest_step(color, v, nv))
              break;
            v = nv;

            if (board[v].size() + moved_size > 8)
              continue;

            Tower source_after = board[source];
            Tower destination_after = board[v];
            int before_count = tower_small_count(source_after) + tower_small_count(destination_after);
            int before_dist = tower_small_distance(source_after, source) + tower_small_distance(destination_after, v);

            for (int i = 0; i < moved_size; ++i) {
              int c = source_after.back();
              source_after.pop_back();
              destination_after.push_back(c);
            }
            home_tower(source_after, source);
            home_tower(destination_after, v);

            // recoveryでも3色以上の複雑なsmall towerは作らない。
            if (popcount_colors(top_small_color_mask(destination_after)) > MIXED_MAX_COLORS) {
              continue;
            }
            if (top_small_transitions(destination_after) > 2)
              continue;

            int after_count = tower_small_count(source_after) + tower_small_count(destination_after);
            int after_dist = tower_small_distance(source_after, source) + tower_small_distance(destination_after, v);
            int homed = before_count - after_count;
            int gain = before_dist - after_dist;

            long long score = 15000LL * homed + 300LL * gain + 3LL * l;
            result.push_back({source, v, moved_size, score, gain, homed});
          }
        }
      }
    }

    sort(result.begin(), result.end(), [](const MixedMove &a, const MixedMove &b) {
      if (a.score != b.score)
        return a.score > b.score;
      if (a.homed != b.homed)
        return a.homed > b.homed;
      if (a.distance_gain != b.distance_gain) {
        return a.distance_gain > b.distance_gain;
      }
      if (a.moved_size != b.moved_size)
        return a.moved_size > b.moved_size;
      if (a.source != b.source)
        return a.source < b.source;
      return a.destination < b.destination;
    });

    return result;
  }

  vector<MixedMove> search_mixed_recovery(const Board &start_board,
                                          const unordered_set<uint64_t> &global_visited) const {
    const int initial_remaining = count_small_slimes(start_board);
    const int initial_distance = small_distance_sum(start_board);

    RecoveryState initial;
    initial.board = start_board;
    initial.remaining = initial_remaining;
    initial.distance_sum = initial_distance;
    initial.score = 0;
    initial.hash = hash_board(start_board);

    vector<RecoveryState> beam{initial};
    optional<RecoveryState> best_progress;

    for (int depth = 0; depth < MIXED_RECOVERY_DEPTH; ++depth) {
      vector<RecoveryState> next;
      unordered_set<uint64_t> seen;
      seen.reserve(MIXED_RECOVERY_BEAM_WIDTH * 32);

      for (const auto &state : beam) {
        auto moves = generate_recovery_moves(state.board);
        for (const auto &move : moves) {
          RecoveryState child = state;
          apply_move_unchecked(child.board, move.source, move.moved_size, move.destination);
          child.moves[child.move_count++] = move;
          child.remaining = count_small_slimes(child.board);
          child.distance_sum = small_distance_sum(child.board);
          child.hash = hash_board(child.board);

          if (!seen.insert(child.hash).second)
            continue;
          if (global_visited.contains(child.hash))
            continue;

          child.score = 100000LL * (initial_remaining - child.remaining) +
                        1000LL * (initial_distance - child.distance_sum) - 10LL * child.move_count;

          // 帰巣が進んだ、または通常mixed戦略が再開できるならrecovery終了。
          bool recovered = child.remaining < initial_remaining;
          if (!recovered) {
            auto normal = generate_mixed_moves(child.board);
            recovered = !normal.empty();
          }

          if (recovered) {
            vector<MixedMove> result;
            result.reserve(child.move_count);
            for (int i = 0; i < child.move_count; ++i) {
              result.push_back(child.moves[i]);
            }
            return result;
          }

          if (child.distance_sum < initial_distance &&
              (!best_progress.has_value() || child.score > best_progress->score)) {
            best_progress = child;
          }

          next.push_back(std::move(child));
        }
      }

      if (next.empty())
        break;

      sort(next.begin(), next.end(), [](const RecoveryState &a, const RecoveryState &b) {
        if (a.score != b.score)
          return a.score > b.score;
        if (a.remaining != b.remaining)
          return a.remaining < b.remaining;
        if (a.distance_sum != b.distance_sum) {
          return a.distance_sum < b.distance_sum;
        }
        return a.hash < b.hash;
      });
      if ((int)next.size() > MIXED_RECOVERY_BEAM_WIDTH) {
        next.resize(MIXED_RECOVERY_BEAM_WIDTH);
      }
      beam = std::move(next);
    }

    // 通常戦略までは復活しなくても、巣距離を単調に改善できたrecoveryなら採用。
    if (best_progress.has_value()) {
      vector<MixedMove> result;
      result.reserve(best_progress->move_count);
      for (int i = 0; i < best_progress->move_count; ++i) {
        result.push_back(best_progress->moves[i]);
      }
      return result;
    }

    return {};
  }

  MixedPhaseResult build_small_mixed_prefix() const {
    MixedPhaseResult result;
    result.board = initial_board();
    result.actions.reserve(512);

    int small_color_count = 0;
    for (int color = 0; color < P.K; ++color) {
      small_color_count += is_small_color(color);
    }
    if (small_color_count == 0) {
      result.success = true;
      return result;
    }

    unordered_set<uint64_t> visited;
    visited.reserve(8192);
    visited.insert(hash_board(result.board));

    for (int step = 0; step < MIXED_MAX_STEPS; ++step) {
      if (count_small_slimes(result.board) == 0) {
        result.success = true;
        return result;
      }

      auto plan = search_mixed_beam(result.board, visited);
      if (!plan.empty()) {
        for (const auto &move : plan) {
          if (!execute_action(result.board, result.actions, move.source, move.moved_size, move.destination)) {
            return result;
          }
          ++result.normal_moves;
          visited.insert(hash_board(result.board));

          if (count_small_slimes(result.board) == 0) {
            result.success = true;
            return result;
          }
        }
        continue;
      }

      // 通常mixedで進めなくなったときだけ、一時的に単色最短路recoveryへ切り替える。
      ++result.recovery_calls;
      auto recovery = search_mixed_recovery(result.board, visited);
      if (recovery.empty())
        return result;

      for (const auto &recovery_move : recovery) {
        if (!execute_action(result.board, result.actions, recovery_move.source, recovery_move.moved_size,
                            recovery_move.destination)) {
          return result;
        }
        ++result.recovery_moves;
        visited.insert(hash_board(result.board));
      }
    }

    return result;
  }

  /**
   * source から根方向の「次の集約点」を返す。
   *
   * ・分岐点
   * ・巣
   *
   * のうち最初に現れるもの。
   */
  int next_merge_point(int color, int source) const {
    const auto &tree = trees[color];
    int v = tree.parent[source];

    while (v != -1) {
      if (v == nest[color])
        return v;
      if (tree.child_count[v] >= 2)
        return v;
      v = tree.parent[v];
    }

    return -1;
  }

  /**
   * full blockを動かす場合に合法な範囲で、
   * 固定木上を根方向へできるだけ長くジャンプする。
   *
   * ただし、
   * ・木が曲がる
   * ・次の分岐点/巣に着く
   * ・同色towerを見つける
   * のいずれかで止める。
   *
   * 容量不足なら -1 を返し、1辺移動側の処理に任せる。
   */
  int find_jump_destination(const Board &board, int color, int source, int moved_size) const {
    const auto &tree = trees[color];

    int first_parent = tree.parent[source];
    if (first_parent < 0)
      return -1;

    int direction = direction_between(source, first_parent);
    if (direction == -1)
      return -1;

    int h = board[source].size();
    int k = h - moved_size;
    int max_jump = k + 1;

    int merge_point = next_merge_point(color, source);

    vector<int> candidates;
    int v = source;

    for (int l = 1; l <= max_jump; ++l) {
      int p = tree.parent[v];
      if (p < 0)
        break;

      if (direction_between(v, p) != direction)
        break;

      candidates.push_back(p);
      v = p;

      if (!board[v].empty() && top_color(board, v) == color)
        break;
      if (v == merge_point)
        break;
    }

    for (int i = (int)candidates.size() - 1; i >= 0; --i) {
      int destination = candidates[i];
      if ((int)board[destination].size() + moved_size <= 8) {
        return destination;
      }
    }

    return -1;
  }

  /**
   * flush packet 専用のジャンプ先探索。
   *
   * 通常輸送はGreedy Steiner treeに従うが、flush中だけは木を外れ、
   * 巣への最短距離が1マスごとに必ず減少する直線だけを使う。
   * したがって選ばれたジャンプ全体も必ず最短路上にある。
   *
   * packet_sizeは論理的に固定し、同色towerへ着地しても増やさない。
   */
  int find_flush_jump_destination(const Board &board, int color, int source, int packet_size) const {
    if (dist_to_nest[color][source] == INT_MAX)
      return -1;

    int h = board[source].size();
    int k = h - packet_size;
    int max_jump = k + 1;

    int best = -1;
    int best_length = -1;
    int best_base_height = -1;

    for (int d = 0; d < 4; ++d) {
      int v = source;

      for (int l = 1; l <= max_jump; ++l) {
        int nv = next_cell(v, d);
        if (nv == -1)
          break;

        // 途中の各1マスが最短路方向でなければ、この方向は終了。
        if (!is_shortest_step(color, v, nv))
          break;

        v = nv;

        if ((int)board[v].size() + packet_size > 8)
          continue;

        int base_height = board[v].size();

        // まず長いジャンプを優先。
        // 同じ長さなら、次のジャンプ力を得やすい高い台を優先。
        if (l > best_length || (l == best_length && base_height > best_base_height) ||
            (l == best_length && base_height == best_base_height && v < best)) {
          best = v;
          best_length = l;
          best_base_height = base_height;
        }
      }
    }

    return best;
  }

  vector<int> shortest_next_cells(const Board &board, int color, int source, int packet_size) const {
    vector<int> result;

    for (int d = 0; d < 4; ++d) {
      int v = next_cell(source, d);
      if (v == -1)
        continue;
      if (!is_shortest_step(color, source, v))
        continue;
      result.push_back(v);
    }

    sort(result.begin(), result.end(), [&](int a, int b) {
      bool fit_a = (int)board[a].size() + packet_size <= 8;
      bool fit_b = (int)board[b].size() + packet_size <= 8;
      if (fit_a != fit_b)
        return fit_a > fit_b;

      int ha = board[a].size();
      int hb = board[b].size();
      if (ha != hb)
        return ha > hb;
      return a < b;
    });

    return result;
  }

  /**
   * packet_size 匹を論理的にマージせず、そのまま巣まで送る。
   *
   * flush の本体では packet_size=4 を使う。
   * 同色towerへ着地しても、次の操作で動かすのは上の packet_size 匹だけ。
   *
   * 前方の同色towerが容量不足なら、そのtowerの上4匹を先に flush して
   * 空きを作ってから元のpacketを再開する。
   */
  bool flush_packet_to_nest_core(Board &board, vector<Action> &actions, int color, int source, int packet_size) const {
    if (packet_size <= 0)
      return true;
    if ((int)actions.size() >= MAX_ACTIONS)
      return false;

    if (source == nest[color]) {
      home_cell(board, source);
      return true;
    }

    if (board[source].empty())
      return false;
    if (top_color(board, source) != color)
      return false;
    if (top_block_size(board, source) < packet_size)
      return false;
    if (dist_to_nest[color][source] == INT_MAX)
      return false;

    int destination = find_flush_jump_destination(board, color, source, packet_size);

    if (destination != -1) {
      if (!execute_action(board, actions, source, packet_size, destination)) {
        return false;
      }

      if (destination == nest[color])
        return true;

      return flush_packet_to_nest_core(board, actions, color, destination, packet_size);
    }

    // 直線ジャンプができない場合も、Steiner treeには戻らず、
    // 最短距離が1減る隣接マスのどれかへ進む。
    vector<int> nexts = shortest_next_cells(board, color, source, packet_size);

    // そのままpacket全体を受けられる最短路隣接点を優先。
    for (int destination2 : nexts) {
      int capacity = 8 - (int)board[destination2].size();
      if (capacity < packet_size)
        continue;

      if (!execute_action(board, actions, source, packet_size, destination2)) {
        continue;
      }

      if (destination2 == nest[color])
        return true;

      return flush_packet_to_nest_core(board, actions, color, destination2, packet_size);
    }

    // すべて容量不足なら、最短路方向にある同色towerを先にflushして
    // 空きを作れる経路を試す。
    for (int destination2 : nexts) {
      if (board[destination2].empty())
        continue;
      if (top_color(board, destination2) != color)
        continue;

      int front_block = top_block_size(board, destination2);
      if (front_block <= 0)
        continue;

      int front_flush = min(strategy_flush_size, front_block);
      Board board_backup = board;
      size_t action_size_backup = actions.size();

      if (!flush_packet_to_nest_core(board, actions, color, destination2, front_flush)) {
        board = std::move(board_backup);
        actions.resize(action_size_backup);
        continue;
      }

      return flush_packet_to_nest_core(board, actions, color, source, packet_size);
    }

    return false;
  }

  /**
   * evaluate_flush_cost() 専用の action 適用。
   *
   * 実際の Action 列は不要なので vector<Action> を生成せず、
   * 実行手数だけを action_count に加算する。
   * 盤面更新・合法性判定・帰巣処理は execute_action() と同じ。
   */
  bool execute_action_count_only(Board &board, int &action_count, int source, int moved_size, int destination) const {
    if (action_count >= MAX_ACTIONS)
      return false;
    if (board[source].empty())
      return false;
    if (!(1 <= moved_size && moved_size <= (int)board[source].size()))
      return false;
    if ((int)board[destination].size() + moved_size > 8)
      return false;

    auto [sx, sy] = cell_pos(source);
    auto [tx, ty] = cell_pos(destination);

    int d = -1;
    int l = -1;

    if (sx == tx) {
      if (sy < ty) {
        d = 3;
        l = ty - sy;
      } else if (ty < sy) {
        d = 2;
        l = sy - ty;
      }
    } else if (sy == ty) {
      if (sx < tx) {
        d = 1;
        l = tx - sx;
      } else if (tx < sx) {
        d = 0;
        l = sx - tx;
      }
    }

    if (d == -1 || l <= 0)
      return false;

    int h = board[source].size();
    int k = h - moved_size;

    if (!(0 <= k && k < h))
      return false;
    if (l > k + 1)
      return false;

    int v = source;
    for (int step = 0; step < l; ++step) {
      v = next_cell(v, d);
      if (v == -1)
        return false;
    }
    if (v != destination)
      return false;

    ++action_count;

    for (int i = 0; i < moved_size; ++i) {
      int color = board[source].back();
      board[source].pop_back();
      board[destination].push_back(color);
    }

    home_cell(board, source);
    home_cell(board, destination);

    return true;
  }

  /**
   * flush_packet_to_nest_core() の count-only 版。
   *
   * 遷移選択は通常版と完全に同じにし、Action オブジェクトだけを作らない。
   * evaluate_flush_cost() から大量に呼ばれるため、探索情報を変えずに
   * vector<Action> の確保・拡張・Action書き込みを除去する。
   */
  bool flush_packet_to_nest_count_only(Board &board, int &action_count, int color, int source, int packet_size) const {
    if (packet_size <= 0)
      return true;
    if (action_count >= MAX_ACTIONS)
      return false;

    if (source == nest[color]) {
      home_cell(board, source);
      return true;
    }

    if (board[source].empty())
      return false;
    if (top_color(board, source) != color)
      return false;
    if (top_block_size(board, source) < packet_size)
      return false;
    if (dist_to_nest[color][source] == INT_MAX)
      return false;

    int destination = find_flush_jump_destination(board, color, source, packet_size);

    if (destination != -1) {
      if (!execute_action_count_only(board, action_count, source, packet_size, destination)) {
        return false;
      }

      if (destination == nest[color])
        return true;

      return flush_packet_to_nest_count_only(board, action_count, color, destination, packet_size);
    }

    vector<int> nexts = shortest_next_cells(board, color, source, packet_size);

    for (int destination2 : nexts) {
      int capacity = 8 - (int)board[destination2].size();
      if (capacity < packet_size)
        continue;

      if (!execute_action_count_only(board, action_count, source, packet_size, destination2)) {
        continue;
      }

      if (destination2 == nest[color])
        return true;

      return flush_packet_to_nest_count_only(board, action_count, color, destination2, packet_size);
    }

    for (int destination2 : nexts) {
      if (board[destination2].empty())
        continue;
      if (top_color(board, destination2) != color)
        continue;

      int front_block = top_block_size(board, destination2);
      if (front_block <= 0)
        continue;

      int front_flush = min(strategy_flush_size, front_block);
      Board board_backup = board;
      int action_count_backup = action_count;

      if (!flush_packet_to_nest_count_only(board, action_count, color, destination2, front_flush)) {
        board = board_backup;
        action_count = action_count_backup;
        continue;
      }

      return flush_packet_to_nest_count_only(board, action_count, color, source, packet_size);
    }

    return false;
  }

  enum class LocalMoveType : uint8_t {
    Flush,
    Setup,
    BorrowCarry,
    BorrowDetach,
  };

  struct LocalMove {
    LocalMoveType type = LocalMoveType::Setup;
    int source = -1;
    int destination = -1;
    int moved_size = 0;
  };

  struct LocalState {
    Board board{};

    // 局所探索の深さは固定なので、操作列も固定長で持つ。
    // vector の heap allocation / copy を避ける。
    array<LocalMove, LOCAL_SEARCH_DEPTH> moves{};
    uint8_t move_count = 0;

    // board / flush_pos / flush_finished から計算した状態hash。
    // beam選抜時に盤面全体を再走査してhashを作り直さない。
    uint64_t hash = 0;

    // 論理的なflushの現在位置。
    int flush_pos = -1;
    bool flush_finished = false;

    // ここから最短路ベースの決定的flushを行った場合の残り手数。
    int finish_cost = INT_MAX;

    // move_count + finish_cost。
    int total_cost = INT_MAX;

    // 現在盤面に残っているactive colorの匹数。
    // reuse_creditのfuture_use_capを差分更新するために保持する。
    int active_remaining = 0;

    // 後続flushで再利用できるplatformの将来価値。
    int reuse_credit = 0;

    // total_cost - (初期盤面から増えたreuse_credit)。
    // 局所探索ではこの値も比較に使う。
    int effective_cost = INT_MAX;

    // beamの多様性維持用。大きいほど「将来platformになりそう」。
    int potential = 0;

    // borrowed-helper 近傍の途中状態。
    // helper_origin は attach 直後（最初のcarry前）の位置。
    // shared_steps が奇数なら helper がtop、偶数なら active がtop。
    int borrow_helper_color = -1;
    int borrow_helper_size = 0;
    int borrow_origin = -1;
    int borrow_shared_steps = 0;

    // helperを切り離したことで将来のその色の輸送が何手程度短くなるかのcredit。
    int borrow_credit = 0;
  };

  /**
   * flush が今後通り得る「最短路領域」を列挙する。
   *
   * Steiner tree上の1本の経路ではなく、sourceから巣までの
   * いずれかの最短路に含まれ、かつsourceからHORIZON以内のセルを全て含む。
   */
  void build_flush_route_cache() {
    flush_route_cache.assign(P.K, vector<vector<int>>(V));
    flush_route_mask_cache.assign(P.K, vector<array<uint8_t, MAX_CELLS>>(V));

    for (int color = 0; color < P.K; ++color) {
      for (int source = 0; source < V; ++source) {
        auto &route = flush_route_cache[color][source];
        auto &mask = flush_route_mask_cache[color][source];
        mask.fill(0);

        if (dist_to_nest[color][source] == INT_MAX)
          continue;

        for (int v = 0; v < V; ++v) {
          if (v == source)
            continue;
          if (floor_dist[source][v] == INT_MAX)
            continue;
          if (floor_dist[source][v] > LOCAL_ROUTE_HORIZON)
            continue;
          if (!on_some_shortest_path(color, source, v))
            continue;

          route.push_back(v);
          mask[v] = 1;
        }

        sort(route.begin(), route.end(), [&](int a, int b) {
          int da = floor_dist[source][a];
          int db = floor_dist[source][b];
          if (da != db)
            return da < db;
          if (dist_to_nest[color][a] != dist_to_nest[color][b]) {
            return dist_to_nest[color][a] > dist_to_nest[color][b];
          }
          return a < b;
        });
      }
    }
  }

  const vector<int> &flush_route_cells(int color, int source) const {
    return flush_route_cache[color][source];
  }

  const array<uint8_t, MAX_CELLS> &flush_route_mask(int color, int source) const {
    return flush_route_mask_cache[color][source];
  }

  /**
   * setup対象色のGreedy Steiner treeで v からparent方向へ進んだとき、
   * flushの最短路領域へ初めて到達するまでの辺数。
   */
  int steps_to_route(int color, int v, const array<uint8_t, MAX_CELLS> &on_route, int max_steps) const {
    if (!trees[color].used[v])
      return -1;

    int cur = v;
    for (int step = 0; step <= max_steps; ++step) {
      if (on_route[cur])
        return step;

      int p = trees[color].parent[cur];
      if (p < 0)
        break;
      cur = p;
    }

    return -1;
  }

  /**
   * 現在の盤面で、platformが後続flushにも使われる見込みを概算する。
   *
   * platformが、残っているactive colorスライムの位置から巣への
   * いずれかの最短路上にあるなら、そのスライム数を需要として数える。
   * 4匹につきflushを1回とみなし、現在のflush1回分を除いた回数を
   * 「再利用回数」とする。
   */
  int estimated_extra_platform_uses(const Board &board, int active_color, int platform) const {
    (void)board;
    if (platform < 0 || platform >= V)
      return 0;

    int demand = shortest_path_demand[active_color][platform];
    return max(0, demand / strategy_flush_size - 1);
  }

  int count_color_in_tower(const Tower &tower, int color) const {
    int result = 0;
    for (int i = 0; i < tower.size(); ++i) {
      result += (tower[i] == color);
    }
    return result;
  }

  int future_platform_use_cap(int active_remaining) const {
    return max(0, active_remaining / strategy_flush_size - 1);
  }

  /**
   * 1セルが reusable_platform_credit に寄与する値。
   *
   * future_use_cap が変わらない遷移では source / destination の2セルだけ
   * 再計算すれば、盤面全走査なしでcreditを厳密に差分更新できる。
   */
  int reusable_platform_cell_credit(const Board &board, int active_color, int v, int future_use_cap) const {
    if (future_use_cap <= 0)
      return 0;
    if (v < 0 || v >= V)
      return 0;
    if (board[v].empty())
      return 0;

    int color = top_color(board, v);
    if (color < 0 || color == active_color)
      return 0;
    if (top_block_size(board, v) != (int)board[v].size())
      return 0;

    int h = board[v].size();
    int platform_max_height = 8 - strategy_flush_size;
    if (!(1 <= h && h <= platform_max_height))
      return 0;

    int extra_uses = min(estimated_extra_platform_uses(board, active_color, v), future_use_cap);
    return extra_uses * h * REUSE_CREDIT_PER_LEVEL;
  }

  /**
   * 盤面上に残る再利用可能platformの将来価値。
   *
   * active_remaining は呼び出し側で保持し、count_color() の盤面全走査を
   * 毎回行わない。future_use_cap が変わった場合の再構築時だけ全セルを見る。
   */
  int reusable_platform_credit(const Board &board, int active_color, int active_remaining) const {
    int future_use_cap = future_platform_use_cap(active_remaining);
    if (future_use_cap == 0)
      return 0;

    int credit = 0;
    for (int v = 0; v < V; ++v) {
      credit += reusable_platform_cell_credit(board, active_color, v, future_use_cap);
    }
    return credit;
  }

  /**
   * flush自身が次に行える操作を全て列挙する。
   * packet_size=4は論理的に固定し、同色towerへ着地しても
   * packetを大きくしない。
   */
  vector<LocalMove> generate_flush_moves(const Board &board, int color, int flush_pos) const {
    vector<LocalMove> result;

    if (flush_pos == nest[color])
      return result;
    if (board[flush_pos].empty())
      return result;
    if (top_color(board, flush_pos) != color)
      return result;
    if (top_block_size(board, flush_pos) < strategy_flush_size)
      return result;
    if (dist_to_nest[color][flush_pos] == INT_MAX)
      return result;

    int h = board[flush_pos].size();
    int k = h - strategy_flush_size;
    int max_jump = k + 1;

    struct Candidate {
      LocalMove move;
      int length = 0;
      int base_height = 0;
      int reuse = 0;
    };
    vector<Candidate> candidates;

    for (int d = 0; d < 4; ++d) {
      int v = flush_pos;

      for (int l = 1; l <= max_jump; ++l) {
        int nv = next_cell(v, d);
        if (nv == -1)
          break;
        if (!is_shortest_step(color, v, nv))
          break;

        v = nv;

        if ((int)board[v].size() + strategy_flush_size > 8)
          continue;

        int reuse = 0;
        if (!board[v].empty() && top_color(board, v) != color && top_block_size(board, v) == (int)board[v].size() &&
            (int)board[v].size() <= 8 - strategy_flush_size) {
          reuse = estimated_extra_platform_uses(board, color, v);
        }

        candidates.push_back({
            {LocalMoveType::Flush, flush_pos, v, strategy_flush_size},
            l,
            (int)board[v].size(),
            reuse,
        });
      }
    }

    // 直接の進行量を最優先し、同程度なら再利用価値のあるplatform着地を優先。
    sort(candidates.begin(), candidates.end(), [](const Candidate &a, const Candidate &b) {
      if (a.length != b.length)
        return a.length > b.length;
      if (a.reuse != b.reuse)
        return a.reuse > b.reuse;
      if (a.base_height != b.base_height)
        return a.base_height > b.base_height;
      return a.move.destination < b.move.destination;
    });

    result.reserve(candidates.size());
    for (const auto &candidate : candidates) {
      result.push_back(candidate.move);
    }

    return result;
  }

  /**
   * 他色をplatform候補へ運ぶ1手を列挙する。
   *
   * 以前は「flush最短路領域に隣接したスライムを1手で直接載せる」だけだった。
   * 今回は最大5手程度先読みするため、他色自身のGreedy Steiner treeを
   * parent方向へ1辺ずつ進める中間操作も探索対象にする。
   *
   * 条件:
   * ・active colorは動かさない
   * ・移動元は純粋な同色tower
   * ・移動先も空、または同じ色の純粋tower
   * ・setup中のtower高さは4以下
   * ・その色のparent列をたどると、残り探索手数以内にflush最短路領域へ到達する
   *
   * これにより、例えば経路から4マス離れた他色スライムも
   * 4手使ってplatform位置まで運べる。
   */
  vector<LocalMove> generate_setup_moves(const Board &board, int active_color, int flush_pos,
                                         int remaining_depth) const {
    vector<LocalMove> result;
    if (remaining_depth <= 0)
      return result;

    const auto &on_route = flush_route_mask(active_color, flush_pos);

    struct CandidateMove {
      LocalMove move;
      int route_dist = INT_MAX;
      int reaches_route = 0;
      int moved_size = 0;
    };
    vector<CandidateMove> candidates;

    for (int source = 0; source < V; ++source) {
      if (source == flush_pos)
        continue;
      if (board[source].empty())
        continue;

      int color = top_color(board, source);
      if (color < 0 || color == active_color)
        continue;

      // setupで扱うtowerは純粋towerだけ。
      if (top_block_size(board, source) != (int)board[source].size()) {
        continue;
      }

      if (!trees[color].used[source])
        continue;

      int destination = trees[color].parent[source];
      if (destination < 0)
        continue;
      if (destination == flush_pos)
        continue;

      // 自身の巣へ入れると消えてplatform材料にならない。
      if (destination == nest[color])
        continue;

      // sourceからrouteへ向かうparent列が、残り手数内に存在するものだけ。
      int source_dist = steps_to_route(color, source, on_route, remaining_depth);
      if (source_dist < 0)
        continue;

      int destination_dist = steps_to_route(color, destination, on_route, remaining_depth);

      // route上のtowerをroute外へ持ち出さない。
      // route外なら、必ず1歩routeへ近づく操作だけを許す。
      if (source_dist == 0) {
        if (destination_dist != 0)
          continue;
      } else {
        if (destination_dist != source_dist - 1)
          continue;
      }

      int destination_color = top_color(board, destination);

      // active colorを他色で覆わない。
      if (destination_color == active_color)
        continue;

      // 空マス、または同色の純粋towerにだけ積む。
      if (!board[destination].empty()) {
        if (destination_color != color)
          continue;
        if (top_block_size(board, destination) != (int)board[destination].size()) {
          continue;
        }
      }

      // flush packetを載せたとき高さ8以下になるよう、
      // setup towerは高さ (8 - flush_size) 以下に抑える。
      int capacity = (8 - strategy_flush_size) - (int)board[destination].size();
      if (capacity <= 0)
        continue;

      int moved_size = min((int)board[source].size(), capacity);
      if (moved_size <= 0)
        continue;

      CandidateMove candidate;
      candidate.move = {
          LocalMoveType::Setup,
          source,
          destination,
          moved_size,
      };
      candidate.route_dist = destination_dist;
      candidate.reaches_route = (destination_dist == 0);
      candidate.moved_size = moved_size;
      candidates.push_back(candidate);
    }

    // 分岐数を抑える。
    // まずrouteへ実際に到達する手、次にrouteまで近い手を優先。
    sort(candidates.begin(), candidates.end(), [](const CandidateMove &a, const CandidateMove &b) {
      if (a.reaches_route != b.reaches_route) {
        return a.reaches_route > b.reaches_route;
      }
      if (a.route_dist != b.route_dist) {
        return a.route_dist < b.route_dist;
      }
      if (a.moved_size != b.moved_size) {
        return a.moved_size > b.moved_size;
      }
      if (a.move.destination != b.move.destination) {
        return a.move.destination < b.move.destination;
      }
      return a.move.source < b.move.source;
    });

    if ((int)candidates.size() > LOCAL_SETUP_CANDIDATE_LIMIT) {
      candidates.resize(LOCAL_SETUP_CANDIDATE_LIMIT);
    }

    result.reserve(candidates.size());
    for (const auto &candidate : candidates) {
      result.push_back(candidate.move);
    }
    return result;
  }

  // flush_pos のtowerが [helper...][active packet...] の2blockだけから成るかを調べる。
  // 既存Flushが純粋な他色towerへ着地した直後がこの形になる。
  bool detect_borrow_attach_state(const Board &board, int active_color, int flush_pos, int &helper_color,
                                  int &helper_size) const {
    helper_color = -1;
    helper_size = 0;
    if (flush_pos < 0 || flush_pos >= V)
      return false;
    const Tower &tower = board[flush_pos];
    if (tower.size() <= strategy_flush_size)
      return false;
    if (top_color(board, flush_pos) != active_color)
      return false;
    if (top_block_size(board, flush_pos) != strategy_flush_size)
      return false;

    int lower = tower.size() - strategy_flush_size;
    int c = tower[0];
    if (c == active_color)
      return false;
    for (int i = 0; i < lower; ++i) {
      if (tower[i] != c)
        return false;
    }
    for (int i = lower; i < tower.size(); ++i) {
      if (tower[i] != active_color)
        return false;
    }

    if (lower <= 0 || lower > 8 - strategy_flush_size)
      return false;
    helper_color = c;
    helper_size = lower;
    return true;
  }

  // borrowed segment が現在towerの全体を占めていることを確認する。
  // Carry先を空マスに限定しているため、borrow開始後はこの形を維持できる。
  bool validate_borrow_segment(const LocalState &state, int active_color) const {
    if (state.borrow_helper_color < 0)
      return false;
    if (state.flush_pos < 0 || state.flush_pos >= V)
      return false;
    const Tower &tower = state.board[state.flush_pos];
    int expected = strategy_flush_size + state.borrow_helper_size;
    if (tower.size() != expected)
      return false;

    bool helper_top = (state.borrow_shared_steps & 1);
    if (!helper_top) {
      for (int i = 0; i < state.borrow_helper_size; ++i) {
        if (tower[i] != state.borrow_helper_color)
          return false;
      }
      for (int i = state.borrow_helper_size; i < expected; ++i) {
        if (tower[i] != active_color)
          return false;
      }
    } else {
      for (int i = 0; i < strategy_flush_size; ++i) {
        if (tower[i] != active_color)
          return false;
      }
      for (int i = strategy_flush_size; i < expected; ++i) {
        if (tower[i] != state.borrow_helper_color)
          return false;
      }
    }
    return true;
  }

  // borrowed-helper 中に許す操作を列挙する。
  // ・Carry: activeの最短路方向へ mixed segment 全体を1マス運ぶ。
  // ・Detach: helperがtopの奇数carry後、helperだけを自身のtreeへ戻す。
  vector<LocalMove> generate_borrow_moves(const LocalState &state, int active_color) const {
    vector<LocalMove> result;
    if (!borrow_neighbor_enabled)
      return result;

    int helper_color = state.borrow_helper_color;
    int helper_size = state.borrow_helper_size;
    int shared_steps = state.borrow_shared_steps;

    // borrow開始前なら、純粋helperへの着地直後の形を検出する。
    if (helper_color < 0) {
      if (!detect_borrow_attach_state(state.board, active_color, state.flush_pos, helper_color, helper_size)) {
        return result;
      }
      shared_steps = 0;
    } else if (!validate_borrow_segment(state, active_color)) {
      return result;
    }

    int segment_size = strategy_flush_size + helper_size;
    bool helper_top = (shared_steps & 1);

    // Carry は空マスへの1歩だけに限定する。
    // mixed segmentの上下関係を正確に追え、既存盤面を不用意に巻き込まない。
    if (shared_steps < BORROW_MAX_SHARED_STEPS) {
      for (int d = 0; d < 4; ++d) {
        int destination = next_cell(state.flush_pos, d);
        if (destination == -1)
          continue;
        if (!is_shortest_step(active_color, state.flush_pos, destination))
          continue;
        if (!state.board[destination].empty())
          continue;

        // nest上では自動帰巣でsegment構造が変わるため、borrow中は避ける。
        int nc = nest_color[destination];
        if (nc == active_color || nc == helper_color)
          continue;

        result.push_back({
            LocalMoveType::BorrowCarry,
            state.flush_pos,
            destination,
            segment_size,
        });
      }
    }

    // helperがtopに来るのは奇数回carry後。そのタイミングだけ切り離せる。
    if (!helper_top)
      return result;

    const Tower &tower = state.board[state.flush_pos];
    int k = tower.size() - helper_size;
    int max_jump = k + 1;

    struct DetachCandidate {
      LocalMove move;
      int gain = INT_MIN;
      int length = 0;
      int merge_height = 0;
    };
    vector<DetachCandidate> candidates;

    int origin_dist = dist_to_nest[helper_color][state.borrow_origin];

    for (int d = 0; d < 4; ++d) {
      int v = state.flush_pos;
      for (int l = 1; l <= max_jump; ++l) {
        int nv = next_cell(v, d);
        if (nv == -1)
          break;
        v = nv;

        if ((int)state.board[v].size() + helper_size > 8)
          continue;

        // 後でhelper色をcolor-fixedで処理できるよう、helper自身のtreeへ戻す。
        if (v != nest[helper_color] && !trees[helper_color].used[v])
          continue;

        // detach後はhelperをpure towerとして残す（巣ならそのまま帰巣）。
        if (v != nest[helper_color] && !state.board[v].empty()) {
          if (top_color(state.board, v) != helper_color)
            continue;
          if (top_block_size(state.board, v) != (int)state.board[v].size())
            continue;
        }

        int after_dist = dist_to_nest[helper_color][v];
        if (after_dist == INT_MAX || origin_dist == INT_MAX)
          continue;
        int gain = origin_dist - after_dist;

        // helperを元より遠くへ捨てる近傍は採らない。
        if (gain < 0)
          continue;

        candidates.push_back({
            {LocalMoveType::BorrowDetach, state.flush_pos, v, helper_size},
            gain,
            l,
            (int)state.board[v].size(),
        });
      }
    }

    sort(candidates.begin(), candidates.end(), [](const DetachCandidate &a, const DetachCandidate &b) {
      if (a.gain != b.gain)
        return a.gain > b.gain;
      if (a.length != b.length)
        return a.length > b.length;
      if (a.merge_height != b.merge_height)
        return a.merge_height > b.merge_height;
      return a.move.destination < b.move.destination;
    });

    if ((int)candidates.size() > BORROW_DETACH_CANDIDATE_LIMIT) {
      candidates.resize(BORROW_DETACH_CANDIDATE_LIMIT);
    }
    for (const auto &candidate : candidates)
      result.push_back(candidate.move);

    return result;
  }

  int borrow_potential(const LocalState &state, int active_color) const {
    if (!borrow_neighbor_enabled)
      return 0;

    if (state.borrow_helper_color >= 0) {
      // borrow途中をbeam後半から落とさない。
      int score = 200000;
      if (state.borrow_shared_steps & 1)
        score += 100000; // detach可能
      return score;
    }

    int helper_color = -1, helper_size = 0;
    if (detect_borrow_attach_state(state.board, active_color, state.flush_pos, helper_color, helper_size)) {
      return 60000 + 5000 * helper_size;
    }
    return 0;
  }

  int evaluate_flush_cost(const Board &board, int color, int source, int packet_size) const {
    if (source == nest[color])
      return 0;

    Board copied = board;
    int action_count = 0;

    if (!flush_packet_to_nest_count_only(copied, action_count, color, source, packet_size)) {
      return INT_MAX;
    }

    return action_count;
  }

  /**
   * beamのtie-break用。
   *
   * ・flush経路上に既に存在する高さ1..4の他色tower
   * ・数手以内にflush経路へ到達できる他色tower
   *
   * を高く評価する。
   * 「最初の数手ではまだflush手数が減らないが、数手後にplatformが完成する」
   * 状態をbeamから落としにくくするための値。
   */
  int platform_potential(const Board &board, int active_color, int flush_pos, int remaining_depth,
                         int reuse_credit) const {
    if (flush_pos == nest[active_color])
      return 0;

    const auto &route = flush_route_cells(active_color, flush_pos);
    const auto &on_route = flush_route_mask(active_color, flush_pos);

    int score = 0;

    // すでに最短路領域上にあるplatform。
    for (int v : route) {
      if (board[v].empty())
        continue;

      int color = top_color(board, v);
      if (color == active_color)
        continue;
      if (top_block_size(board, v) != (int)board[v].size())
        continue;
      if ((int)board[v].size() > 8 - strategy_flush_size)
        continue;

      int h = board[v].size();
      int progress = floor_dist[flush_pos][v];
      int extra_uses = estimated_extra_platform_uses(board, active_color, v);

      score += 100 * h;
      score += max(0, 30 - progress);

      // 後続flushでも使えそうな台は強く残す。
      score += 160 * extra_uses * h;
    }

    if (remaining_depth > 0) {
      // まだrouteに到達していないが、数手でplatformになれるtower。
      for (int v = 0; v < V; ++v) {
        if (board[v].empty())
          continue;
        int color = top_color(board, v);
        if (color < 0 || color == active_color)
          continue;
        if (top_block_size(board, v) != (int)board[v].size())
          continue;
        if ((int)board[v].size() > 8 - strategy_flush_size)
          continue;
        if (on_route[v])
          continue;

        int dist = steps_to_route(color, v, on_route, remaining_depth);
        if (dist <= 0)
          continue;

        score += max(0, remaining_depth - dist + 1) * (int)board[v].size() * 4;
      }
    }

    // route以外も含め、後続flushで再利用されるplatform全体の価値を加える。
    // reuse_credit は LocalState で厳密に差分更新済み。
    score += 80 * reuse_credit;

    return score;
  }

  uint64_t hash_local_state(const LocalState &state) const {
    uint64_t h = 1469598103934665603ULL;
    auto mix = [&](uint64_t x) {
      h ^= x + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
      h *= 1099511628211ULL;
    };

    mix((uint64_t)(state.flush_pos + 1));
    mix((uint64_t)state.flush_finished);
    mix((uint64_t)(state.borrow_helper_color + 2));
    mix((uint64_t)(state.borrow_helper_size + 1));
    mix((uint64_t)(state.borrow_origin + 2));
    mix((uint64_t)(state.borrow_shared_steps + 1));
    mix((uint64_t)(state.borrow_credit + 1));

    for (int v = 0; v < V; ++v) {
      if (state.board[v].empty())
        continue;
      mix((uint64_t)(v + 1));
      mix((uint64_t)state.board[v].size());
      for (int i = 0; i < state.board[v].size(); ++i) {
        mix((uint64_t)(state.board[v][i] + 1));
      }
    }

    return h;
  }

  /**
   * 「setupを先に全部行ってからflush」ではなく、
   * flush操作と他色setup操作を同じ探索木に入れて最大5手程度先読みする。
   *
   * 例えば
   *   setup -> setup -> flush -> setup -> flush -> ...
   * のような途中挿入も探索できる。
   *
   * 各探索状態からは、最後に最短路ベースの決定的flushを行った場合の
   * 残り手数を正確に計算する。
   * したがって評価値は
   *
   *   ここまでの局所操作数 + 残りflush手数
   *
   * となる。
   */
  vector<LocalMove> search_platform_plan(const Board &board, int color, int source) const {
    int baseline = evaluate_flush_cost(board, color, source, strategy_flush_size);
    if (baseline == INT_MAX)
      return {};

    vector<LocalMove> best_plan;

    const int baseline_active_remaining = count_color(board, color);
    const int baseline_reuse_credit = reusable_platform_credit(board, color, baseline_active_remaining);
    int best_effective = baseline;
    int best_total = baseline;

    LocalState initial;
    initial.board = board;
    initial.flush_pos = source;
    initial.flush_finished = false;
    initial.finish_cost = baseline;
    initial.total_cost = baseline;
    initial.active_remaining = baseline_active_remaining;
    initial.reuse_credit = baseline_reuse_credit;
    initial.effective_cost = baseline;
    initial.potential = platform_potential(board, color, source, LOCAL_SEARCH_DEPTH, initial.reuse_credit);
    initial.potential += borrow_potential(initial, color);
    initial.hash = hash_local_state(initial);

    vector<LocalState> beam;
    beam.push_back(std::move(initial));

    for (int depth = 0; depth < LOCAL_SEARCH_DEPTH; ++depth) {
      vector<LocalState> next;
      next.reserve(LOCAL_BEAM_WIDTH * 12);
      unordered_set<uint64_t> seen;
      seen.reserve(LOCAL_BEAM_WIDTH * 32);

      for (const auto &state : beam) {
        if (state.flush_finished)
          continue;

        vector<LocalMove> moves;

        int remaining_after_this = LOCAL_SEARCH_DEPTH - (depth + 1);

        if (borrow_neighbor_enabled && state.borrow_helper_color >= 0) {
          // borrow中はCarry/Detachだけを許し、途中で別操作へ逸れない。
          moves = generate_borrow_moves(state, color);
        } else {
          moves = generate_flush_moves(state.board, color, state.flush_pos);

          auto setup_moves = generate_setup_moves(state.board, color, state.flush_pos, remaining_after_this + 1);
          moves.insert(moves.end(), setup_moves.begin(), setup_moves.end());

          if (borrow_neighbor_enabled) {
            auto borrow_moves = generate_borrow_moves(state, color);
            moves.insert(moves.end(), borrow_moves.begin(), borrow_moves.end());
          }
        }

        for (const auto &move : moves) {
          LocalState child;
          child.board = state.board;
          child.moves = state.moves;
          child.move_count = state.move_count;
          child.flush_pos = state.flush_pos;
          child.flush_finished = state.flush_finished;
          child.active_remaining = state.active_remaining;
          child.borrow_helper_color = state.borrow_helper_color;
          child.borrow_helper_size = state.borrow_helper_size;
          child.borrow_origin = state.borrow_origin;
          child.borrow_shared_steps = state.borrow_shared_steps;
          child.borrow_credit = state.borrow_credit;

          // 1手で盤面が変わるのは source / destination の2セルだけ。
          // active color残数とreuse creditを、この2セルの差分だけで更新する。
          const int old_future_use_cap = future_platform_use_cap(state.active_remaining);

          int active_before = count_color_in_tower(state.board[move.source], color);
          int old_credit = reusable_platform_cell_credit(state.board, color, move.source, old_future_use_cap);

          if (move.destination != move.source) {
            active_before += count_color_in_tower(state.board[move.destination], color);
            old_credit += reusable_platform_cell_credit(state.board, color, move.destination, old_future_use_cap);
          }

          vector<Action> dummy;
          if (!execute_action(child.board, dummy, move.source, move.moved_size, move.destination)) {
            continue;
          }

          int active_after = count_color_in_tower(child.board[move.source], color);
          if (move.destination != move.source) {
            active_after += count_color_in_tower(child.board[move.destination], color);
          }
          child.active_remaining += active_after - active_before;

          const int new_future_use_cap = future_platform_use_cap(child.active_remaining);

          if (new_future_use_cap == old_future_use_cap) {
            int new_credit = reusable_platform_cell_credit(child.board, color, move.source, new_future_use_cap);
            if (move.destination != move.source) {
              new_credit += reusable_platform_cell_credit(child.board, color, move.destination, new_future_use_cap);
            }
            child.reuse_credit = state.reuse_credit - old_credit + new_credit;
          } else {
            // flushが巣へ到達してfuture_use_cap自体が変化した場合は、
            // 全platformのclamp値が変わり得るため、この稀な場合だけ全走査する。
            child.reuse_credit = reusable_platform_credit(child.board, color, child.active_remaining);
          }

          if (child.move_count >= LOCAL_SEARCH_DEPTH)
            continue;
          child.moves[child.move_count++] = move;

          if (move.type == LocalMoveType::Flush) {
            child.flush_pos = move.destination;
            if (move.destination == nest[color]) {
              child.flush_finished = true;
            }
          } else if (move.type == LocalMoveType::BorrowCarry) {
            if (child.borrow_helper_color < 0) {
              int helper_color = -1, helper_size = 0;
              if (!detect_borrow_attach_state(state.board, color, state.flush_pos, helper_color, helper_size)) {
                continue;
              }
              child.borrow_helper_color = helper_color;
              child.borrow_helper_size = helper_size;
              child.borrow_origin = state.flush_pos;
              child.borrow_shared_steps = 0;
            }
            ++child.borrow_shared_steps;
            child.flush_pos = move.destination;
          } else if (move.type == LocalMoveType::BorrowDetach) {
            if (child.borrow_helper_color < 0)
              continue;
            int helper_color = child.borrow_helper_color;
            int origin_dist = dist_to_nest[helper_color][child.borrow_origin];
            int after_dist = dist_to_nest[helper_color][move.destination];
            if (origin_dist != INT_MAX && after_dist != INT_MAX) {
              int gain = max(0, origin_dist - after_dist);
              child.borrow_credit += min(BORROW_FUTURE_CREDIT_CAP, gain);
            }
            child.borrow_helper_color = -1;
            child.borrow_helper_size = 0;
            child.borrow_origin = -1;
            child.borrow_shared_steps = 0;
            // active packet は元のflush_posに残る。
          }

          // 同一depthでは move_count は全状態で同じ。
          // board / flush_pos / flush_finished が同じなら、以降の評価値と
          // 探索可能な遷移も同じなので、重い評価より先に重複排除する。
          child.hash = hash_local_state(child);
          if (!seen.insert(child.hash).second)
            continue;

          if (child.flush_finished) {
            child.finish_cost = 0;
          } else {
            child.finish_cost = evaluate_flush_cost(child.board, color, child.flush_pos, strategy_flush_size);
          }

          if (child.finish_cost == INT_MAX) {
            child.total_cost = INT_MAX;
            child.effective_cost = INT_MAX;
          } else {
            child.total_cost = (int)child.move_count + child.finish_cost;

            int extra_reuse_credit = max(0, child.reuse_credit - baseline_reuse_credit);

            child.effective_cost = child.total_cost - extra_reuse_credit - child.borrow_credit;

            // borrow途中（helperをまだ切り離していない）ではplanを確定しない。
            // helperを自身のtreeへ戻した後だけ、通常flushへ復帰できる。
            if (child.borrow_helper_color < 0 &&
                (child.effective_cost < best_effective ||
                 (child.effective_cost == best_effective && child.total_cost < best_total))) {
              best_effective = child.effective_cost;
              best_total = child.total_cost;
              best_plan.assign(child.moves.begin(), child.moves.begin() + child.move_count);
            }
          }

          child.potential =
              platform_potential(child.board, color, child.flush_pos, remaining_after_this, child.reuse_credit);
          child.potential += borrow_potential(child, color);

          next.push_back(std::move(child));
        }
      }

      if (next.empty())
        break;

      // まず「現在すでに良い」状態を半分残す。
      sort(next.begin(), next.end(), [](const LocalState &a, const LocalState &b) {
        if (a.effective_cost != b.effective_cost) {
          return a.effective_cost < b.effective_cost;
        }
        if (a.total_cost != b.total_cost)
          return a.total_cost < b.total_cost;
        if (a.finish_cost != b.finish_cost)
          return a.finish_cost < b.finish_cost;
        if (a.reuse_credit != b.reuse_credit)
          return a.reuse_credit > b.reuse_credit;
        if (a.potential != b.potential)
          return a.potential > b.potential;
        return a.move_count < b.move_count;
      });

      vector<LocalState> selected;
      selected.reserve(LOCAL_BEAM_WIDTH);
      unordered_set<uint64_t> selected_hash;
      selected_hash.reserve(LOCAL_BEAM_WIDTH * 2);

      int cost_quota = LOCAL_BEAM_WIDTH / 2;
      for (const auto &state : next) {
        if ((int)selected.size() >= cost_quota)
          break;
        if (!selected_hash.insert(state.hash).second)
          continue;
        selected.push_back(state);
      }

      // 残り半分は、まだ直接得になっていなくてもplatform形成の
      // 見込みが大きい状態を残す。
      sort(next.begin(), next.end(), [](const LocalState &a, const LocalState &b) {
        if (a.potential != b.potential)
          return a.potential > b.potential;
        if (a.reuse_credit != b.reuse_credit)
          return a.reuse_credit > b.reuse_credit;
        if (a.effective_cost != b.effective_cost) {
          return a.effective_cost < b.effective_cost;
        }
        if (a.total_cost != b.total_cost)
          return a.total_cost < b.total_cost;
        if (a.finish_cost != b.finish_cost)
          return a.finish_cost < b.finish_cost;
        return a.move_count < b.move_count;
      });

      for (const auto &state : next) {
        if ((int)selected.size() >= LOCAL_BEAM_WIDTH)
          break;
        if (!selected_hash.insert(state.hash).second)
          continue;
        selected.push_back(state);
      }

      beam = std::move(selected);
    }

    return best_plan;
  }

  /**
   * flush packetをflushする。
   *
   * 最大5手程度、flush自身の移動と他色のsetupを混ぜて局所探索する。
   * 探索で得なprefixが見つかった場合はその操作列を実行し、
   * 残りは最短路ベースの決定的flushで巣まで送る。
   */
  bool flush_with_platform_search(Board &board, vector<Action> &actions, int color, int source) const {
    if (top_block_size(board, source) < strategy_flush_size)
      return false;

    vector<LocalMove> plan = search_platform_plan(board, color, source);
    int flush_pos = source;

    for (const auto &move : plan) {
      if (!execute_action(board, actions, move.source, move.moved_size, move.destination)) {
        return false;
      }

      if (move.type == LocalMoveType::Flush || move.type == LocalMoveType::BorrowCarry) {
        flush_pos = move.destination;
        if (flush_pos == nest[color]) {
          return true;
        }
      }
    }

    return flush_packet_to_nest_core(board, actions, color, flush_pos, strategy_flush_size);
  }

  /**
   * active color のtop blockを1段階進める。
   *
   * 基本はGreedy Steiner tree上を通常マージ。
   * tower全体が高さ8になったら、上X段だけをflushして
   * 下側をfill用の集積blockとして残す。
   *
   * また、前方同色towerへfull blockを載せられない場合は、
   * currentの一部をparentへ入れて高さ8までfillした後、
   * parentの上X段をflushする。
   */
  bool advance_one(Board &board, vector<Action> &actions, int color, int source) const {
    if (source == nest[color])
      return false;
    if (board[source].empty())
      return false;
    if (top_color(board, source) != color)
      return false;
    if (!trees[color].used[source])
      return false;

    int block_size = top_block_size(board, source);
    if (block_size <= 0)
      return false;

    // 高さ8のtowerができたら、上X段を先に流す。
    // 下側（または下側の残り）は後続blockを受けるfill地点として残す。
    if ((int)board[source].size() == 8 && block_size >= strategy_flush_size) {
      return flush_with_platform_search(board, actions, color, source);
    }

    // full blockのまま進めるなら通常のSteiner集約を優先する。
    int jump_destination = find_jump_destination(board, color, source, block_size);

    if (jump_destination != -1) {
      return execute_action(board, actions, source, block_size, jump_destination);
    }

    int parent = trees[color].parent[source];
    if (parent < 0)
      return false;

    int capacity = 8 - (int)board[parent].size();

    // 同色の前方towerへfull blockが入らない。
    // currentから必要分をfillして高さ8を作り、上X段をflushする。
    if (!board[parent].empty() && top_color(board, parent) == color && capacity < block_size) {
      int need = capacity;

      if (need > 0) {
        if (!execute_action(board, actions, source, need, parent)) {
          return false;
        }
      }

      if ((int)board[parent].size() != 8)
        return false;
      if (top_block_size(board, parent) < strategy_flush_size)
        return false;

      return flush_with_platform_search(board, actions, color, parent);
    }

    if (capacity <= 0)
      return false;

    // 他色tower上では入る分だけ載せる。
    // 局所探索で作るplatformは高さ4以下なので、
    // 4匹blockなら最大高さ8のジャンプ台になる。
    int moved_size = min(block_size, capacity);
    if (moved_size <= 0)
      return false;

    return execute_action(board, actions, source, moved_size, parent);
  }

  // =========================================================
  // One color
  // =========================================================

  int deepest_active_cell(const Board &board, int color) const {
    int best = -1;
    int best_depth = -1;
    int best_block_size = -1;

    for (int v = 0; v < V; ++v) {
      if (board[v].empty())
        continue;
      if (top_color(board, v) != color)
        continue;
      if (!trees[color].used[v])
        continue;
      if (v == nest[color])
        continue;

      int depth = trees[color].depth[v];
      int block_size = top_block_size(board, v);

      // 葉側から処理して、分岐点で自然にマージさせる。
      if (depth > best_depth || (depth == best_depth && block_size < best_block_size) ||
          (depth == best_depth && block_size == best_block_size && v < best)) {
        best = v;
        best_depth = depth;
        best_block_size = block_size;
      }
    }

    return best;
  }

  bool solve_one_color(Board &board, vector<Action> &actions, int color) const {
    while (count_color(board, color) > 0) {
      if ((int)actions.size() >= MAX_ACTIONS)
        return false;

      int source = deepest_active_cell(board, color);

      // current color が盤面に残っているのにtopに存在しないなら失敗。
      // 一色ずつ処理する設計では原理上起きない。
      if (source == -1)
        return false;

      if (!advance_one(board, actions, color, source))
        return false;
    }

    return true;
  }

  // =========================================================
  // One color order
  // =========================================================

  SimulationResult simulate(const vector<int> &color_order) const {
    Board board = initial_board();
    vector<Action> actions;
    actions.reserve(4096);

    for (int color : color_order) {
      if (!solve_one_color(board, actions, color)) {
        return {false, {}};
      }
    }

    if (count_all(board) != 0) {
      return {false, {}};
    }

    return {true, std::move(actions)};
  }

  // small colorsをcolor-mixedで全て帰巣させた盤面から、
  // 残ったlarge colorsだけ既存のcolor-fixed + Greedy Steinerで処理する。
  SimulationResult simulate_from_mixed_prefix(const vector<int> &color_order, const MixedPhaseResult &prefix) const {
    if (!prefix.success)
      return {false, {}};

    Board board = prefix.board;
    vector<Action> actions = prefix.actions;
    actions.reserve(4096);

    for (int color : color_order) {
      if (count_color(board, color) == 0)
        continue;
      if (!solve_one_color(board, actions, color)) {
        return {false, {}};
      }
    }

    if (count_all(board) != 0)
      return {false, {}};
    return {true, std::move(actions)};
  }

  // =========================================================
  // Color order search
  // =========================================================

  void try_order(const vector<int> &order, int flush_size, set<vector<int>> &seen, long long &trial_count,
                 long long &borrow_win_count, SimulationResult &strategy_best, SimulationResult &global_best) {
    if (!seen.insert(order).second)
      return;
    ++trial_count;

    strategy_flush_size = flush_size;

    // pure color-fixed を必ずbaselineとして残す。
    borrow_neighbor_enabled = false;
    auto result = simulate(order);

    // 同じ色順・flush sizeで borrowed-helper 近傍あり版も試す。
    borrow_neighbor_enabled = true;
    auto borrowed = simulate(order);
    borrow_neighbor_enabled = false;

    if (borrowed.success && (!result.success || borrowed.actions.size() < result.actions.size())) {
      result = std::move(borrowed);
      ++borrow_win_count;
    }

    if (!result.success)
      return;

    if (!strategy_best.success || result.actions.size() < strategy_best.actions.size()) {
      strategy_best = result;
    }

    if (!global_best.success || result.actions.size() < global_best.actions.size()) {
      global_best = std::move(result);
    }
  }

  long long permutation_count_cap(int n, long long cap) const {
    long long result = 1;
    for (int i = 2; i <= n; ++i) {
      if (result > cap / i)
        return cap;
      result *= i;
    }
    return result;
  }

  vector<Action> solve() {
    constexpr array<int, 4> FLUSH_SIZES = {7, 6, 5, 4};

    SimulationResult best;
    array<SimulationResult, 4> strategy_best;
    array<set<vector<int>>, 4> seen;
    array<long long, 4> trial_count{};
    array<long long, 4> borrow_win_count{};

    // color_mixed はいったん休止し、pure color-fixed と
    // borrowed-helper 近傍あり color-fixed だけを比較する。

    vector<vector<int>> seed_orders;

    vector<int> order(P.K);
    iota(order.begin(), order.end(), 0);
    seed_orders.push_back(order);

    reverse(order.begin(), order.end());
    seed_orders.push_back(order);

    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int a, int b) {
      if (terminals[a].size() != terminals[b].size()) {
        return terminals[a].size() > terminals[b].size();
      }
      return a < b;
    });
    seed_orders.push_back(order);

    reverse(order.begin(), order.end());
    seed_orders.push_back(order);

    /**
     * まず4戦略すべてを少なくとも1回は試す。
     * これにより fill8/flush7,6,5,4 のどれかが未評価のまま
     * 時間切れになることを避ける。
     */
    for (int i = 0; i < 4; ++i) {
      try_order(seed_orders[0], FLUSH_SIZES[i], seen[i], trial_count[i], borrow_win_count[i], strategy_best[i], best);
    }

    // 残りの代表的な色順も、戦略間で公平にround-robinする。
    for (int seed = 1; seed < (int)seed_orders.size(); ++seed) {
      for (int i = 0; i < 4; ++i) {
        if (time_over())
          break;
        try_order(seed_orders[seed], FLUSH_SIZES[i], seen[i], trial_count[i], borrow_win_count[i], strategy_best[i],
                  best);
      }
      if (time_over())
        break;
    }

    if (P.K >= 2) {
      const long long all_orders = permutation_count_cap(P.K, 1000000000LL);

      int strategy_index = 0;

      // 残り時間は4戦略をround-robinしながら色順探索に使う。
      while (!time_over()) {
        bool all_finished = true;
        for (int i = 0; i < 4; ++i) {
          if ((long long)seen[i].size() < all_orders) {
            all_finished = false;
            break;
          }
        }
        if (all_finished)
          break;

        int i = strategy_index;
        strategy_index = (strategy_index + 1) % 4;

        if ((long long)seen[i].size() == all_orders)
          continue;

        iota(order.begin(), order.end(), 0);
        shuffle(order.begin(), order.end(), mt);

        try_order(order, FLUSH_SIZES[i], seen[i], trial_count[i], borrow_win_count[i], strategy_best[i], best);
      }
    }

    long long total_trials = 0;
    for (int i = 0; i < 4; ++i) {
      total_trials += trial_count[i];
      cerr << "fill8/flush" << FLUSH_SIZES[i] << ": ";
      if (strategy_best[i].success) {
        cerr << strategy_best[i].actions.size() << " actions";
      } else {
        cerr << "no complete solution";
      }
      cerr << ", trials=" << trial_count[i] << ", borrow_wins=" << borrow_win_count[i] << '\n';
    }
    cerr << "total trials=" << total_trials << '\n';

    if (!best.success) {
      // 念のため時間制限とは無関係に、flush4 + 基本色順を最後に試す。
      strategy_flush_size = 4;
      borrow_neighbor_enabled = false;
      best = simulate(seed_orders[0]);
    }

    return best.actions;
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  Solver solver;
  auto answer = solver.solve();

  for (const auto &action : answer) {
    cout << action << '\n';
  }

  return 0;
}
