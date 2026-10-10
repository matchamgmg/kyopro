#include <bits/stdc++.h>
using namespace std;

// AHC072 experimental solver:
// - no Steiner tree
// - no color-fixed phase
// - no mixed solver / profit graph
// - forward policy uses ONLY transport gain
// - past repair is event-driven; v1 implements SupportReady only

constexpr int MAX_N = 20;
constexpr int MAX_CELLS = MAX_N * MAX_N;
constexpr int MAX_HEIGHT = 8;
constexpr int MAX_ACTIONS = 100000;
constexpr int TIME_LIMIT_MS = 1900;

constexpr int REPAIR_LOOKBACK = 12;
constexpr int REPAIR_BEAM_WIDTH = 64;
constexpr int REPAIR_MAX_EXTRA_SUPPORT = 3;
constexpr int REPAIR_SUPPORT_CANDIDATES = 10;
constexpr int REPAIR_COUNTERFACTUAL_CANDIDATES = 6;
constexpr int REPAIR_MAX_EDITS = 5;
constexpr int RECENT_HASH_LIMIT = 32;

constexpr array<int, 4> DX = {-1, 1, 0, 0};
constexpr array<int, 4> DY = {0, 0, -1, 1};
constexpr array<char, 4> DIR_CHAR = {'U', 'D', 'L', 'R'};

struct Slime {
  uint16_t id = 0;
  uint8_t color = 0;

  bool operator==(const Slime &) const = default;
};

struct Tower {
  array<Slime, MAX_HEIGHT> a{};
  uint8_t n = 0;

  int size() const {
    return n;
  }
  bool empty() const {
    return n == 0;
  }
  Slime &operator[](int i) {
    return a[i];
  }
  const Slime &operator[](int i) const {
    return a[i];
  }
  Slime &back() {
    return a[n - 1];
  }
  const Slime &back() const {
    return a[n - 1];
  }
  void push_back(Slime s) {
    a[n++] = s;
  }
  void pop_back() {
    --n;
  }

  bool operator==(const Tower &rhs) const {
    if (n != rhs.n)
      return false;
    for (int i = 0; i < n; ++i) {
      if (!(a[i] == rhs.a[i]))
        return false;
    }
    return true;
  }
};

using Board = array<Tower, MAX_CELLS>;

struct MoveSpec {
  int source = -1;
  int destination = -1;
  int moved_size = 0;
};

struct OutputAction {
  int x = 0;
  int y = 0;
  int k = 0;
  int d = 0;
  int l = 0;
};

struct AppliedMove {
  bool ok = false;
  long long gain = LLONG_MIN;
  OutputAction out{};
  array<uint16_t, MAX_HEIGHT> moved_ids{};
  uint8_t moved_count = 0;
};

struct HistoryEntry {
  MoveSpec spec{};
  OutputAction out{};
  long long gain = 0;
  array<uint16_t, MAX_HEIGHT> moved_ids{};
  uint8_t moved_count = 0;
  Tower old_source{};
  Tower old_destination{};
};

struct SupportReadyEvent {
  int cell = -1;
  int required_base = 0;
  int deadline = -1;                        // history index: event must hold BEFORE this action.
  array<uint16_t, MAX_HEIGHT> target_ids{}; // bottom -> top target segment
  uint8_t target_count = 0;
};

struct Counterfactual {
  MoveSpec spec{};
  long long gain = LLONG_MIN;
  SupportReadyEvent event{};
  int support_deficit = 0;
};

struct CandidateMove {
  MoveSpec spec{};
  long long gain = LLONG_MIN;
};

struct RepairState {
  Board board{};
  vector<MoveSpec> actions;
  long long gain = 0;
  int edits = 0;
  bool event_reached = false;
  uint64_t hash = 0;
};

struct RepairResult {
  bool success = false;
  int start_index = -1;
  vector<MoveSpec> actions;
  MoveSpec final_counterfactual{};
  long long repaired_gain = LLONG_MIN;
  int repaired_actions = INT_MAX;
  long long original_gain = LLONG_MIN;
  int original_actions = INT_MAX;
};

struct Solver {
  int N, K, V;
  vector<string> C;
  Board board{};
  array<int, 12> nest{};
  array<int, MAX_CELLS> nest_color{};
  vector<array<int, MAX_CELLS>> dist_to_nest;
  vector<HistoryEntry> history;
  deque<uint64_t> recent_hashes;
  chrono::steady_clock::time_point start_time;

  Solver() : start_time(chrono::steady_clock::now()) {
    cin >> N >> K;
    V = N * N;
    C.resize(N);
    for (auto &row : C)
      cin >> row;

    nest.fill(-1);
    nest_color.fill(-1);

    uint16_t next_id = 1;
    for (int x = 0; x < N; ++x) {
      for (int y = 0; y < N; ++y) {
        char ch = C[x][y];
        int v = id(x, y);
        if ('A' <= ch && ch <= 'L') {
          int color = ch - 'A';
          nest[color] = v;
          nest_color[v] = color;
        } else if ('a' <= ch && ch <= 'l') {
          int color = ch - 'a';
          board[v].push_back(Slime{next_id++, (uint8_t)color});
        }
      }
    }
    build_distances();
    recent_hashes.push_back(hash_board(board));
  }

  int id(int x, int y) const {
    return x * N + y;
  }
  int x_of(int v) const {
    return v / N;
  }
  int y_of(int v) const {
    return v % N;
  }
  bool in_bounds(int x, int y) const {
    return 0 <= x && x < N && 0 <= y && y < N;
  }
  bool floor_cell(int v) const {
    int x = x_of(v), y = y_of(v);
    return C[x][y] != '#';
  }

  bool time_over() const {
    return chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start_time).count() >=
           TIME_LIMIT_MS;
  }

  static uint64_t splitmix64(uint64_t x) {
    x += 0x9e3779b97f4a7c15ULL;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    return x ^ (x >> 31);
  }

  uint64_t hash_board(const Board &b) const {
    uint64_t h = splitmix64((uint64_t)N * 1009 + K);
    for (int v = 0; v < V; ++v) {
      uint64_t z = splitmix64((uint64_t)v + 0x123456789abcdefULL);
      z ^= splitmix64((uint64_t)b[v].size() * 0x9e3779b9ULL + v);
      for (int i = 0; i < b[v].size(); ++i) {
        uint64_t key = ((uint64_t)b[v][i].id << 16) | ((uint64_t)b[v][i].color << 8) | (uint64_t)i;
        z ^= splitmix64(key + (uint64_t)v * 0xbf58476d1ce4e5b9ULL);
      }
      h ^= z;
    }
    return h;
  }

  void build_distances() {
    dist_to_nest.assign(K, {});
    for (int color = 0; color < K; ++color) {
      auto &dist = dist_to_nest[color];
      dist.fill(INT_MAX);
      if (nest[color] < 0)
        continue;
      queue<int> q;
      dist[nest[color]] = 0;
      q.push(nest[color]);
      while (!q.empty()) {
        int v = q.front();
        q.pop();
        int x = x_of(v), y = y_of(v);
        for (int d = 0; d < 4; ++d) {
          int nx = x + DX[d], ny = y + DY[d];
          if (!in_bounds(nx, ny))
            continue;
          int nv = id(nx, ny);
          if (!floor_cell(nv) || dist[nv] != INT_MAX)
            continue;
          dist[nv] = dist[v] + 1;
          q.push(nv);
        }
      }
    }
  }

  int direction_between(int s, int t) const {
    int sx = x_of(s), sy = y_of(s), tx = x_of(t), ty = y_of(t);
    if (sy == ty) {
      if (tx < sx)
        return 0;
      if (tx > sx)
        return 1;
    }
    if (sx == tx) {
      if (ty < sy)
        return 2;
      if (ty > sy)
        return 3;
    }
    return -1;
  }

  int line_length(int s, int t) const {
    int d = direction_between(s, t);
    if (d < 0)
      return -1;
    return abs(x_of(s) - x_of(t)) + abs(y_of(s) - y_of(t));
  }

  bool line_floor_clear(int s, int t) const {
    int d = direction_between(s, t);
    if (d < 0)
      return false;
    int len = line_length(s, t);
    int x = x_of(s), y = y_of(s);
    for (int l = 1; l <= len; ++l) {
      x += DX[d];
      y += DY[d];
      if (!in_bounds(x, y))
        return false;
      int v = id(x, y);
      if (!floor_cell(v))
        return false;
    }
    return true;
  }

  void home_tower(Tower &t, int v) const {
    int nc = nest_color[v];
    if (nc < 0)
      return;
    while (!t.empty() && t.back().color == nc)
      t.pop_back();
  }

  long long transport_gain(const Board &b, const MoveSpec &m) const {
    if (m.source < 0 || m.destination < 0 || m.moved_size <= 0) {
      return LLONG_MIN / 4;
    }
    const Tower &src = b[m.source];
    if (m.moved_size > src.size())
      return LLONG_MIN / 4;
    long long g = 0;
    for (int i = src.size() - m.moved_size; i < src.size(); ++i) {
      int c = src[i].color;
      int ds = dist_to_nest[c][m.source];
      int dt = dist_to_nest[c][m.destination];
      if (ds == INT_MAX || dt == INT_MAX)
        return LLONG_MIN / 4;
      g += ds - dt;
    }
    return g;
  }

  bool legal_move(const Board &b, const MoveSpec &m) const {
    if (m.source < 0 || m.source >= V || m.destination < 0 || m.destination >= V)
      return false;
    if (m.source == m.destination || m.moved_size <= 0)
      return false;
    if (!floor_cell(m.source) || !floor_cell(m.destination))
      return false;
    const Tower &src = b[m.source];
    const Tower &dst = b[m.destination];
    int h = src.size();
    if (m.moved_size > h)
      return false;
    if (dst.size() + m.moved_size > MAX_HEIGHT)
      return false;
    int d = direction_between(m.source, m.destination);
    if (d < 0)
      return false;
    int l = line_length(m.source, m.destination);
    if (l <= 0 || !line_floor_clear(m.source, m.destination))
      return false;
    int k = h - m.moved_size;
    if (!(0 <= k && k < h))
      return false;
    return l <= k + 1;
  }

  AppliedMove apply_move(Board &b, const MoveSpec &m) const {
    AppliedMove res;
    if (!legal_move(b, m))
      return res;

    int h = b[m.source].size();
    int k = h - m.moved_size;
    int d = direction_between(m.source, m.destination);
    int l = line_length(m.source, m.destination);

    res.ok = true;
    res.gain = transport_gain(b, m);
    res.out = OutputAction{x_of(m.source), y_of(m.source), k, d, l};
    res.moved_count = (uint8_t)m.moved_size;
    for (int i = 0; i < m.moved_size; ++i) {
      res.moved_ids[i] = b[m.source][h - m.moved_size + i].id;
    }

    for (int i = 0; i < m.moved_size; ++i) {
      Slime s = b[m.source].back();
      b[m.source].pop_back();
      b[m.destination].push_back(s);
    }
    home_tower(b[m.source], m.source);
    home_tower(b[m.destination], m.destination);
    return res;
  }

  bool execute_and_record(const MoveSpec &m) {
    if ((int)history.size() >= MAX_ACTIONS)
      return false;
    Tower old_s = board[m.source];
    Tower old_d = board[m.destination];
    AppliedMove a = apply_move(board, m);
    if (!a.ok)
      return false;
    HistoryEntry e;
    e.spec = m;
    e.out = a.out;
    e.gain = a.gain;
    e.moved_ids = a.moved_ids;
    e.moved_count = a.moved_count;
    e.old_source = old_s;
    e.old_destination = old_d;
    history.push_back(e);

    uint64_t h = hash_board(board);
    recent_hashes.push_back(h);
    while ((int)recent_hashes.size() > RECENT_HASH_LIMIT)
      recent_hashes.pop_front();
    return true;
  }

  void undo_last(Board &b, const HistoryEntry &e) const {
    b[e.spec.source] = e.old_source;
    b[e.spec.destination] = e.old_destination;
  }

  Board board_at_history_index(int index) const {
    Board b = board;
    for (int i = (int)history.size() - 1; i >= index; --i) {
      undo_last(b, history[i]);
    }
    return b;
  }

  int total_slimes(const Board &b) const {
    int s = 0;
    for (int v = 0; v < V; ++v)
      s += b[v].size();
    return s;
  }

  vector<CandidateMove> enumerate_legal_moves(const Board &b) const {
    vector<CandidateMove> result;
    result.reserve(4096);
    for (int s = 0; s < V; ++s) {
      int h = b[s].size();
      if (h == 0)
        continue;
      int sx = x_of(s), sy = y_of(s);
      for (int moved = 1; moved <= h; ++moved) {
        int max_jump = h - moved + 1;
        for (int d = 0; d < 4; ++d) {
          int x = sx, y = sy;
          for (int l = 1; l <= max_jump; ++l) {
            x += DX[d];
            y += DY[d];
            if (!in_bounds(x, y))
              break;
            int t = id(x, y);
            if (!floor_cell(t))
              break;
            if (b[t].size() + moved > MAX_HEIGHT)
              continue;
            MoveSpec m{s, t, moved};
            long long g = transport_gain(b, m);
            result.push_back({m, g});
          }
        }
      }
    }
    sort(result.begin(), result.end(), [](const CandidateMove &a, const CandidateMove &b) {
      if (a.gain != b.gain)
        return a.gain > b.gain;
      if (a.spec.source != b.spec.source)
        return a.spec.source < b.spec.source;
      if (a.spec.destination != b.spec.destination)
        return a.spec.destination < b.spec.destination;
      return a.spec.moved_size > b.spec.moved_size;
    });
    return result;
  }

  bool is_recent_hash(uint64_t h) const {
    return find(recent_hashes.begin(), recent_hashes.end(), h) != recent_hashes.end();
  }

  optional<CandidateMove> choose_greedy_move() const {
    auto moves = enumerate_legal_moves(board);
    if (moves.empty())
      return nullopt;

    // transport gain is the only score. Recent-state avoidance is merely a loop guard.
    int probe = min<int>(64, moves.size());
    for (int i = 0; i < probe; ++i) {
      Board child = board;
      if (!apply_move(child, moves[i].spec).ok)
        continue;
      if (!is_recent_hash(hash_board(child)))
        return moves[i];
    }
    return moves.front();
  }

  bool segment_contains_id(const Tower &t, int moved_size, const array<uint16_t, MAX_HEIGHT> &ids, int id_count) const {
    if (moved_size > t.size())
      return true;
    for (int i = t.size() - moved_size; i < t.size(); ++i) {
      for (int j = 0; j < id_count; ++j) {
        if (t[i].id == ids[j])
          return true;
      }
    }
    return false;
  }

  int find_last_arrival_step(uint16_t slime_id, int destination) const {
    for (int i = (int)history.size() - 1; i >= 0; --i) {
      const auto &e = history[i];
      if (e.spec.destination != destination)
        continue;
      for (int j = 0; j < e.moved_count; ++j) {
        if (e.moved_ids[j] == slime_id)
          return i;
      }
    }
    return -1; // initial occupant or not found
  }

  vector<Counterfactual> enumerate_support_counterfactuals(long long legal_best_gain) const {
    vector<Counterfactual> result;

    for (int s = 0; s < V; ++s) {
      int h = board[s].size();
      if (h == 0)
        continue;
      int sx = x_of(s), sy = y_of(s);

      for (int moved = 1; moved <= h; ++moved) {
        int base = h - moved;
        for (int d = 0; d < 4; ++d) {
          int x = sx, y = sy;
          for (int l = 1; l <= N - 1; ++l) {
            x += DX[d];
            y += DY[d];
            if (!in_bounds(x, y))
              break;
            int t = id(x, y);
            if (!floor_cell(t))
              break;
            if (board[t].size() + moved > MAX_HEIGHT)
              continue;
            if (l <= base + 1)
              continue; // already legal, not a support counterfactual

            int required_base = l - 1;
            int deficit = required_base - base;
            if (deficit <= 0 || deficit > REPAIR_MAX_EXTRA_SUPPORT)
              continue;

            MoveSpec m{s, t, moved};
            long long g = transport_gain(board, m);
            if (g <= legal_best_gain)
              continue;

            SupportReadyEvent ev;
            ev.cell = s;
            ev.required_base = required_base;
            ev.target_count = (uint8_t)moved;
            for (int i = 0; i < moved; ++i) {
              ev.target_ids[i] = board[s][h - moved + i].id;
            }

            uint16_t bottom_id = ev.target_ids[0];
            int anchor = find_last_arrival_step(bottom_id, s);
            if (anchor < 0)
              continue; // v1 does not move an initially placed target away and back.
            ev.deadline = anchor;

            result.push_back(Counterfactual{m, g, ev, deficit});
          }
        }
      }
    }

    sort(result.begin(), result.end(), [](const Counterfactual &a, const Counterfactual &b) {
      if (a.gain != b.gain)
        return a.gain > b.gain;
      if (a.support_deficit != b.support_deficit) {
        return a.support_deficit < b.support_deficit;
      }
      int la = abs(a.spec.destination - a.spec.source);
      int lb = abs(b.spec.destination - b.spec.source);
      if (la != lb)
        return la > lb;
      if (a.spec.source != b.spec.source)
        return a.spec.source < b.spec.source;
      if (a.spec.destination != b.spec.destination)
        return a.spec.destination < b.spec.destination;
      return a.spec.moved_size > b.spec.moved_size;
    });
    if ((int)result.size() > REPAIR_COUNTERFACTUAL_CANDIDATES) {
      result.resize(REPAIR_COUNTERFACTUAL_CANDIDATES);
    }
    return result;
  }

  bool target_segment_matches(const Board &b, const SupportReadyEvent &ev) const {
    const Tower &t = b[ev.cell];
    if (t.size() < ev.target_count + ev.required_base)
      return false;
    int first = t.size() - ev.target_count;
    if (first < ev.required_base)
      return false;
    for (int i = 0; i < ev.target_count; ++i) {
      if (t[first + i].id != ev.target_ids[i])
        return false;
    }
    return true;
  }

  vector<pair<MoveSpec, long long>> support_moves_to_event(const Board &b, const SupportReadyEvent &ev) const {
    struct Tmp {
      MoveSpec m;
      long long gain;
      int resulting_height;
    };
    vector<Tmp> tmp;

    int u = ev.cell;
    for (int s = 0; s < V; ++s) {
      if (s == u || b[s].empty())
        continue;
      int h = b[s].size();
      for (int moved = 1; moved <= h; ++moved) {
        if (segment_contains_id(b[s], moved, ev.target_ids, ev.target_count))
          continue;
        MoveSpec m{s, u, moved};
        if (!legal_move(b, m))
          continue;
        Board child = b;
        AppliedMove a = apply_move(child, m);
        if (!a.ok)
          continue;
        if (child[u].size() <= b[u].size())
          continue;
        tmp.push_back({m, a.gain, child[u].size()});
      }
    }

    sort(tmp.begin(), tmp.end(), [](const Tmp &a, const Tmp &b) {
      if (a.resulting_height != b.resulting_height) {
        return a.resulting_height > b.resulting_height;
      }
      if (a.gain != b.gain)
        return a.gain > b.gain;
      if (a.m.moved_size != b.m.moved_size)
        return a.m.moved_size > b.m.moved_size;
      if (a.m.source != b.m.source)
        return a.m.source < b.m.source;
      return a.m.destination < b.m.destination;
    });

    vector<pair<MoveSpec, long long>> result;
    int lim = min<int>(REPAIR_SUPPORT_CANDIDATES, tmp.size());
    for (int i = 0; i < lim; ++i)
      result.push_back({tmp[i].m, tmp[i].gain});
    return result;
  }

  static bool better_pre_deadline(const RepairState &a, const RepairState &b, int event_cell, int required_base) {
    int pa = min(a.board[event_cell].size(), required_base);
    int pb = min(b.board[event_cell].size(), required_base);
    if (pa != pb)
      return pa > pb;
    if (a.gain != b.gain)
      return a.gain > b.gain;
    if (a.actions.size() != b.actions.size())
      return a.actions.size() < b.actions.size();
    return a.edits < b.edits;
  }

  static bool better_post_deadline(const RepairState &a, const RepairState &b) {
    if (a.gain != b.gain)
      return a.gain > b.gain;
    if (a.actions.size() != b.actions.size())
      return a.actions.size() < b.actions.size();
    return a.edits < b.edits;
  }

  vector<RepairState> dedup_and_keep(vector<RepairState> states, bool before_deadline,
                                     const SupportReadyEvent &ev) const {
    unordered_map<uint64_t, int> best_index;
    vector<RepairState> uniq;
    uniq.reserve(states.size());

    for (auto &st : states) {
      auto it = best_index.find(st.hash);
      if (it == best_index.end()) {
        int idx = uniq.size();
        best_index.emplace(st.hash, idx);
        uniq.push_back(std::move(st));
      } else {
        int idx = it->second;
        bool better = before_deadline ? better_pre_deadline(st, uniq[idx], ev.cell, ev.required_base)
                                      : better_post_deadline(st, uniq[idx]);
        if (better)
          uniq[idx] = std::move(st);
      }
    }

    auto cmp = [&](const RepairState &a, const RepairState &b) {
      return before_deadline ? better_pre_deadline(a, b, ev.cell, ev.required_base) : better_post_deadline(a, b);
    };
    sort(uniq.begin(), uniq.end(), cmp);
    if ((int)uniq.size() > REPAIR_BEAM_WIDTH)
      uniq.resize(REPAIR_BEAM_WIDTH);
    return uniq;
  }

  optional<RepairResult> search_support_repair(const Counterfactual &cf, const CandidateMove &baseline) const {
    const auto &ev = cf.event;
    int current = history.size();
    int anchor = ev.deadline;
    if (anchor < 0 || anchor >= current)
      return nullopt;

    int start = max(0, anchor - REPAIR_LOOKBACK);
    int deadline_rel = anchor - start;
    Board start_board = board_at_history_index(start);

    long long original_gain = baseline.gain;
    for (int i = start; i < current; ++i)
      original_gain += history[i].gain;
    int original_actions = (current - start) + 1;

    RepairState init;
    init.board = start_board;
    init.hash = hash_board(init.board);
    vector<RepairState> beam{std::move(init)};

    for (int rel = 0; rel < current - start; ++rel) {
      int hist_index = start + rel;
      bool before_deadline = rel < deadline_rel;

      vector<RepairState> next;
      next.reserve(beam.size() * 8);

      for (const auto &st : beam) {
        if (st.edits > REPAIR_MAX_EDITS)
          continue;

        if (rel == deadline_rel && st.board[ev.cell].size() < ev.required_base) {
          continue;
        }

        // 1) Replay the original operation.
        {
          RepairState child = st;
          AppliedMove a = apply_move(child.board, history[hist_index].spec);
          if (a.ok) {
            child.actions.push_back(history[hist_index].spec);
            child.gain += a.gain;
            child.event_reached =
                st.event_reached || (rel >= deadline_rel) || (child.board[ev.cell].size() >= ev.required_base);
            child.hash = hash_board(child.board);
            next.push_back(std::move(child));
          }
        }

        // 2) Skip/erase the original operation.
        if (st.edits < REPAIR_MAX_EDITS) {
          RepairState child = st;
          ++child.edits;
          child.hash = st.hash;
          next.push_back(std::move(child));
        }

        // 3) Before the deadline, replace this slot by an operation that prepares
        //    SupportReady(cell, required_base).
        if (before_deadline && st.edits < REPAIR_MAX_EDITS) {
          auto supports = support_moves_to_event(st.board, ev);
          for (auto &[sm, sg] : supports) {
            RepairState child = st;
            AppliedMove a = apply_move(child.board, sm);
            if (!a.ok)
              continue;
            child.actions.push_back(sm);
            child.gain += a.gain;
            ++child.edits;
            child.hash = hash_board(child.board);
            next.push_back(std::move(child));
          }

          // 4) In one historical slot, perform a support operation and then the
          //    original operation. This costs +1 raw action, but later skips can
          //    compensate for it exactly.
          int combo_lim = min<int>(3, supports.size());
          for (int q = 0; q < combo_lim; ++q) {
            RepairState child = st;
            AppliedMove sa = apply_move(child.board, supports[q].first);
            if (!sa.ok)
              continue;
            AppliedMove oa = apply_move(child.board, history[hist_index].spec);
            if (!oa.ok)
              continue;
            child.actions.push_back(supports[q].first);
            child.actions.push_back(history[hist_index].spec);
            child.gain += sa.gain + oa.gain;
            ++child.edits;
            child.hash = hash_board(child.board);
            next.push_back(std::move(child));
          }
        }
      }

      if (next.empty())
        return nullopt;
      beam = dedup_and_keep(std::move(next), before_deadline, ev);
    }

    optional<RepairResult> best;
    for (auto &st : beam) {
      if (!target_segment_matches(st.board, ev))
        continue;
      if (!legal_move(st.board, cf.spec))
        continue;

      Board after = st.board;
      AppliedMove final = apply_move(after, cf.spec);
      if (!final.ok)
        continue;

      long long repaired_gain = st.gain + final.gain;
      int repaired_actions = (int)st.actions.size() + 1;

      // Conservative adoption: repair must Pareto-dominate the original window.
      bool dominates = (repaired_gain > original_gain && repaired_actions <= original_actions) ||
                       (repaired_gain >= original_gain && repaired_actions < original_actions);
      if (!dominates)
        continue;

      RepairResult rr;
      rr.success = true;
      rr.start_index = start;
      rr.actions = st.actions;
      rr.final_counterfactual = cf.spec;
      rr.repaired_gain = repaired_gain;
      rr.repaired_actions = repaired_actions;
      rr.original_gain = original_gain;
      rr.original_actions = original_actions;

      if (!best.has_value() || rr.repaired_gain > best->repaired_gain ||
          (rr.repaired_gain == best->repaired_gain && rr.repaired_actions < best->repaired_actions)) {
        best = std::move(rr);
      }
    }
    return best;
  }

  bool apply_repair(const RepairResult &rr) {
    if (!rr.success)
      return false;

    while ((int)history.size() > rr.start_index) {
      const auto &e = history.back();
      undo_last(board, e);
      history.pop_back();
    }

    recent_hashes.clear();
    recent_hashes.push_back(hash_board(board));

    for (const auto &m : rr.actions) {
      if (!execute_and_record(m))
        return false;
    }
    if (!execute_and_record(rr.final_counterfactual))
      return false;
    return true;
  }

  bool try_past_repair(const CandidateMove &baseline) {
    auto cfs = enumerate_support_counterfactuals(baseline.gain);
    optional<RepairResult> best;

    for (const auto &cf : cfs) {
      if (time_over())
        break;
      auto rr = search_support_repair(cf, baseline);
      if (!rr.has_value())
        continue;
      if (!best.has_value() || rr->repaired_gain > best->repaired_gain ||
          (rr->repaired_gain == best->repaired_gain && rr->repaired_actions < best->repaired_actions)) {
        best = std::move(rr);
      }
    }

    if (!best.has_value())
      return false;
    return apply_repair(*best);
  }

  vector<OutputAction> solve() {
    int guard = 0;
    while (total_slimes(board) > 0 && (int)history.size() < MAX_ACTIONS && !time_over() && guard++ < MAX_ACTIONS) {
      auto baseline = choose_greedy_move();
      if (!baseline.has_value())
        break;

      if (!execute_and_record(baseline->spec))
        break;
    }

    vector<OutputAction> ans;
    ans.reserve(history.size());
    for (const auto &e : history)
      ans.push_back(e.out);
    return ans;
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  Solver solver;
  auto ans = solver.solve();
  for (const auto &a : ans) {
    cout << a.x << ' ' << a.y << ' ' << a.k << ' ' << DIR_CHAR[a.d] << ' ' << a.l << '\n';
  }
  return 0;
}
