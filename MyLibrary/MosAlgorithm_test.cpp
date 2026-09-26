#include <bits/stdc++.h>
template <class Data, class Aggregate, class Answer, auto Init_ans> struct Mo {
  Mo(const std::vector<Data> &data_, const Aggregate &total_, const std::vector<int> &L_, const std::vector<int> &R_)
      : data_vec(data_), current_total(total_), query_L_vec(L_), query_R_vec(R_) {
    assert(query_L_vec.size() == query_R_vec.size());
    N = data_.size();
    Q = query_L_vec.size();
    answer_vec = std::vector<Answer>(Q, Init_ans());
    index_vec.resize(Q);
    for (int i = 0; i < Q; ++i) {
      index_vec[i] = i;
    }
  }

  void Sort_by_snake_order() {
    Sort_by_snake_order(query_L_vec, query_R_vec);
  }

  void Sort_by_Hilbert_order() {
    int maxN = 1;
    while (maxN <= N) {
      maxN <<= 1;
    }
    std::vector<int64_t> Hilbert_val(Q);
    auto Calc_Hilbert_val = [maxN](int x, int y) -> int64_t {
      int64_t rx, ry, d;
      d = 0;
      for (int64_t s = maxN >> 1; s > 0; s >>= 1) {
        rx = (x & s) > 0;
        ry = (y & s) > 0;
        d += s * s * ((rx * 3) ^ ry);
        if (ry > 0)
          continue;
        if (rx > 0) {
          x = maxN - 1 - x;
          y = maxN - 1 - y;
        }
        std::swap(x, y);
      }
      return d;
    };
    for (int i = 0; i < Q; ++i) {
      Hilbert_val[i] = Calc_Hilbert_val(query_L_vec[i], query_R_vec[i]);
    }
    auto Comp = [&Hilbert_val](int i, int j) -> bool {
      return Hilbert_val[i] < Hilbert_val[j];
    };
    std::sort(index_vec.begin(), index_vec.end(), Comp);
  }

  void Solve_all_query() {
    int current_l = 0, current_r = 0;
    for (int i : index_vec) {
      while (current_l > query_L_vec[i]) {
        --current_l;
        current_total.Add(data_vec, current_l);
      }
      while (current_r < query_R_vec[i]) {
        current_total.Add(data_vec, current_r);
        ++current_r;
      }
      while (current_l < query_L_vec[i]) {
        current_total.Del(data_vec, current_l);
        ++current_l;
      }
      while (current_r > query_R_vec[i]) {
        --current_r;
        current_total.Del(data_vec, current_r);
      }
      answer_vec[i] = current_total.get_answer();
    }
  }

public:
  std::vector<Answer> answer_vec;

private:
  int N;
  int Q;
  Aggregate current_total;
  std::vector<Data> data_vec;
  std::vector<int> query_L_vec, query_R_vec;
  std::vector<int> index_vec;

  void Sort_by_snake_order(std::vector<int> &L, std::vector<int> &R) {
    int divide = std::max(1, int(N / std::sqrt(Q)));
    auto Comp = [&L, &R, divide](int i, int j) -> bool {
      if (L[i] / divide != L[j] / divide) {
        return L[i] < L[j];
      }
      if ((L[i] / divide) & 1) {
        return R[i] > R[j];
      }
      return R[i] < R[j];
    };
    std::sort(index_vec.begin(), index_vec.end(), Comp);
  }
};

using namespace std;

int main() {
  /* ABC242-G */

  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  cin >> N;
  vector<int> A(N);
  for (int &x : A) {
    cin >> x;
    x--;
  }
  int Q;
  cin >> Q;
  vector<int> L(Q), R(Q);
  for (int i = 0; i < Q; ++i) {
    cin >> L[i] >> R[i];
    L[i]--;
  }

  auto Init_ans = []() -> int {
    return 0;
  };
  struct Aggregate {
    int current_answer;
    vector<int> cnt;
    Aggregate() {}
    Aggregate(int N) : cnt(vector<int>(N, 0)), current_answer(0) {}

    int get_answer() {
      return current_answer;
    }
    void Add(const vector<int> &data, int index) {
      cnt[data[index]]++;
      if (cnt[data[index]] % 2 == 0) {
        current_answer++;
      }
    }
    void Del(const vector<int> &data, int index) {
      cnt[data[index]]--;
      if (cnt[data[index]] % 2 == 1) {
        current_answer--;
      }
    }
  };

  Mo<int, Aggregate, int, Init_ans> mo(A, Aggregate(N), L, R);
  mo.Sort_by_snake_order();
  mo.Solve_all_query();
  for (int ans : mo.answer_vec) {
    cout << ans << '\n';
  }
}