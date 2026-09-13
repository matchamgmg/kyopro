/**
 * https://github.com/matchamgmg/kyopro/tree/main
 */

#include <atcoder/all>
#include <bits/stdc++.h>

using namespace std;
using namespace atcoder;
using ll = long long;

#define rep(i, s, n) for (int i = (s); i < (int)(n); i++)
#define rrep(i, s, n) for (int i = (n - 1); i >= (int)(s); i--)
#define all(a) a.begin(), a.end()
#define rall(a) a.rbegin(), a.rend()

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

struct Hall {
  int x;
  int y;
};

struct Brick {
  int x;
  int y;
  int l;
  int idx;
  int base_idx;
};

struct Solver {
  int H = 40;
  int W = 60;
  int K = 120;
  array<int, 5> C;
  array<Hall, 120> halls;
  Solver() {
    input();
  }
  void input() {
    for (int &c : C) {
      cin >> c;
    }
    for (Hall &h : halls) {
      cin >> h.x >> h.y;
    }
  }

  void solve_piller() {
    vector<vector<int>> Wall(H, vector<int>(W, -1));
    vector<vector<int>> Wall2(H, vector<int>(W, 0));
    for (auto &h : halls) {
      Wall2[h.y][h.x] = -2;
    }
    rep(h, 0, H) {
      rep(w, 0, 4) {
        int x = 7 + 15 * w;
        int y = h;
        Wall[y][x] = 1;
      }
    }

    rep(p, 0, 4) {
      int center = 7 + 15 * p;
      rep(h, 0, H) {
        bool exist_7l = false;
        bool exist_7r = false;
        bool exist_9l = false;
        bool exist_9r = false;
        rep(i, -7, -4) {
          exist_7l |= (Wall2[h][center + i] == -2);
        }
        rep(i, 5, 8) {
          exist_7r |= (Wall2[h][center + i] == -2);
        }
        rep(i, -4, 0) {
          exist_9l |= (Wall2[h][center + i] == -2);
        }
        rep(i, 1, 5) {
          exist_9r |= (Wall2[h][center + i] == -2);
        }
        if (exist_7l) {
          Wall[h][center - 4 - 3] = 7;
          if (!exist_7r && exist_9r) {
            Wall[h][center + 4 - 3] = 7;
          }
        }
        if (exist_7r) {
          Wall[h][center + 4 - 3] = 7;
          if (!exist_7l && exist_9l) {
            Wall[h][center - 4 - 3] = 7;
          }
        }
      }
    }

    rep(p, 0, 4) {
      int center = 7 + 15 * p;
      rep(h, 0, H - 2) {
        if (Wall[h][center - 4 - 3] == 7 && Wall[h + 2][center - 4 - 3] == 7 && Wall[h + 1][center - 4 - 3] <= 0) {
          Wall[h + 1][center - 4] = 1;
        }
        if (Wall[h][center + 4 - 3] == 7 && Wall[h + 2][center + 4 - 3] == 7 && Wall[h + 1][center + 4 - 3] <= 0) {
          Wall[h + 1][center + 4] = 1;
        }
      }
    }

    rep(p, 0, 4) {
      int center = 7 + 15 * p;
      rrep(h, 1, H) {
        if (Wall[h][center - 4 - 3] == 7 && Wall[h - 1][center - 4 - 3] <= 0 && Wall[h - 1][center - 4] <= 0 &&
            (Wall[h - 1][center + 4 - 3] == 7 || Wall[h - 1][center + 4] == 1)) {
          Wall[h - 1][center - 4 - 3] = 7;
        }
        if (Wall[h][center + 4 - 3] == 7 && Wall[h - 1][center + 4 - 3] <= 0 && Wall[h - 1][center + 4] <= 0 &&
            (Wall[h - 1][center - 4 - 3] == 7 || Wall[h - 1][center - 4] == 1)) {
          Wall[h - 1][center + 4 - 3] = 7;
        }
      }
    }

    rep(p, 0, 4) {
      int center = 7 + 15 * p;
      rep(h, 0, H - 1) {
        bool oku = (Wall[h][center - 4 - 3] <= 0 && Wall[h][center - 4] <= 0 && Wall[h][center + 4 - 3] <= 0 &&
                    Wall[h][center + 4] <= 0);
        bool okl = (Wall[h + 1][center - 4 - 3] == 7);
        bool okr = (Wall[h + 1][center + 4 - 3] == 7);
        if (oku && (okl || okr)) {
          Wall[h][center] = 0;
          Wall[h][center - 4] = 9;
        }
      }
    }

    rep(p, 0, 4) {
      int center = 7 + 15 * p;
      rep(h, 0, H) {
        bool exist_7l = false;
        bool exist_7r = false;
        bool exist_9 = false;
        rep(i, -7, -4) {
          exist_7l |= (Wall2[h][center + i] == -2);
        }
        rep(i, 5, 8) {
          exist_7r |= (Wall2[h][center + i] == -2);
        }
        rep(i, -4, 5) {
          if (i == 0) {
            continue;
          }
          exist_9 |= (Wall2[h][center + i] == -2);
        }
        if (!exist_7l && !exist_7r && exist_9) {
          Wall[h][center - 4 - 3] = 0;
          Wall[h][center - 4] = 0;
          Wall[h][center + 4 - 3] = 0;
          Wall[h][center + 4] = 0;
          Wall[h][center] = 0;
          Wall[h][center - 4] = 9;
        }
      }
    }

    rep(p, 0, 4) {
      int center = 7 + 15 * p;
      rep(h, 0, H) {
        if (Wall[h][center - 4] == 9 && (h == H - 1 || h < H - 1 && Wall[h + 1][center - 4] == 9)) {
          int l = 0, r = 0;
          rep(i, -4, 0) {
            if (Wall2[h][center + i] == -2) {
              chmax(l, abs(i));
            }
          }
          rep(i, 1, 5) {
            if (Wall2[h][center + i] == -2) {
              chmax(r, abs(i));
            }
          }
          int width = max(l, r);
          Wall[h][center - 4] = 0;
          Wall[h][center - width] = width * 2 + 1;
        }
      }
    }

    rep(p, 0, 4) {
      int center = 7 + 15 * p;
      rep(h, 0, H) {
        int center_l = center - 4;
        int center_r = center + 4;
        if (Wall[h][center_l - 3] == 7) {
          int l = 0, r = 0;
          rep(i, -3, 0) {
            if (Wall2[h][center_l + i] == -2) {
              chmax(l, abs(i));
            }
          }
          rep(i, 1, 4) {
            if (Wall2[h][center_l + i] == -2) {
              chmax(r, abs(i));
            }
          }
          int width = max(l, r);
          Wall[h][center_l - 3] = 0;
          Wall[h][center_l - width] = width * 2 + 1;
        }
        if (Wall[h][center_r - 3] == 7) {
          int l = 0, r = 0;
          rep(i, -3, 0) {
            if (Wall2[h][center_r + i] == -2) {
              chmax(l, abs(i));
            }
          }
          rep(i, 1, 4) {
            if (Wall2[h][center_r + i] == -2) {
              chmax(r, abs(i));
            }
          }
          int width = max(l, r);
          Wall[h][center_r - 3] = 0;
          Wall[h][center_r - width] = width * 2 + 1;
        }
      }
    }

    int cnt = 0;
    rep(h, 0, H) {
      rep(w, 0, W) {
        cnt += (Wall[h][w] > 0);
      }
    }
    cout << cnt << endl;
    rep(h, 0, H) {
      rep(w, 0, W) {
        if (Wall[h][w] <= 0) {
          continue;
        }
        cout << w << ' ' << h << ' ' << Wall[h][w] << endl;
      }
    }
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int h, w, k;
  cin >> h >> w >> k;
  Solver sol;
  sol.solve_piller();
}